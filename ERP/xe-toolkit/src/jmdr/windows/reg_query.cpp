#include <iostream>
#include <windows.h>
#include <math.h>
#include <PdhMsg.h>
#include "mdr_single.h"
#include "mdr_tuple.h"
#include "reg_query.h"
#include "mdr_exception.h"

namespace mdr {

reg_query_t::reg_query_t()
{
  obj_index = 0;
  PDH_STATUS status;

  status = PdhOpenQuery(0, 0, &qh);
  if (status != ERROR_SUCCESS)
    throw mdr_exception_t(
      "reg_query_t::reg_query_t: PdhOpenQuery() fails: %s",
        mdr_exception_t::PdhErrorMessage(status));
}

reg_query_t::~reg_query_t()
{
  PdhCloseQuery(qh);
}

void reg_query_t::add_counter(const char *full_name, PDH_HCOUNTER &counter_handle)
{
  PDH_STATUS status;

  status = PdhAddCounter(qh, full_name, 0, &counter_handle);
  if (status != ERROR_SUCCESS)
    throw mdr_exception_t(
      "reg_query_t::add_counter: PdhAddCounter(%s) fails: %s",
        full_name, mdr_exception_t::PdhErrorMessage(status));
}

void reg_query_t::remove_counter(PDH_HCOUNTER &counter_handle)
{
  PDH_STATUS status;

  status = PdhRemoveCounter(counter_handle);
  if (status != ERROR_SUCCESS)
    throw mdr_exception_t(
      "reg_query_t::remove_counter: PdhRemoveCounter() fails: %s",
        mdr_exception_t::PdhErrorMessage(status));
}

void reg_query_t::perform_query(void)
{
  PDH_STATUS status;

  status = PdhCollectQueryData(qh);
  if (status != ERROR_SUCCESS)
    throw mdr_exception_t(
      "reg_query_t::perform_query: PdhCollectQueryData() fails: %s",
          mdr_exception_t::PdhErrorMessage(status));
}

static void format_error(const char *fmt, ...)
{
  va_list arg;
  char buf[BUFSIZ];

  va_start(arg, fmt);
  _vsnprintf_s(buf, sizeof buf, fmt, arg);
  va_end(arg);
#if throwing_exceptions_didnt_make_the_application_end
  throw pdh_exception_t(buf);
#endif
}

PDH_STATUS reg_query_t::get_counter_value(
  PDH_HCOUNTER counter_handle,
  DWORD format, DWORD *counter_type,
  PDH_FMT_COUNTERVALUE *formatted_value,
  bool is_rate)
{
  PDH_STATUS status;

  // take the first shot
  status = PdhGetFormattedCounterValue(counter_handle,
              format, counter_type, formatted_value);
  // bingo
  if (status == ERROR_SUCCESS)
    return status;

  // If it's this status and it's a rate, then
  // try another collection and see if gets
  // over it.

  if (status == PDH_INVALID_DATA && is_rate) {
    // collection
    status = PdhCollectQueryData(qh);

    // well, then that's that.
    if (status != ERROR_SUCCESS)
      return status;

    // ok, it's now or never.
    return PdhGetFormattedCounterValue(counter_handle,
                format, counter_type, formatted_value);
  }

  // it's some other issue
  return status;
}

void reg_query_t::get_counter_data(PDH_HCOUNTER counter_handle, mdr_value_t &rval)
{
  PDH_STATUS status;
  PDH_RAW_COUNTER counter_value;
  PDH_FMT_COUNTERVALUE formatted_value;
  DWORD counter_type = 0;
  bool is_rate = false;

  const int SIZE_MASK   = PERF_SIZE_VARIABLE_LEN;
  const int USAGE_MASK  = PERF_TYPE_ZERO;
  const int NUMBER_MASK = PERF_NUMBER_DECIMAL | PERF_NUMBER_DEC_1000;
  const int COUNTER_MASK  = PERF_COUNTER_PRECISION;
  const int TEXT_MASK   = PERF_TEXT_ASCII;

  int size;
  int usage;
  int number;
  int counter;
  int text;

  status = PdhGetRawCounterValue(counter_handle, &counter_type, &counter_value);
  if (status != ERROR_SUCCESS)
    throw mdr_exception_t(
      "reg_query_t::perform_query: "
        "PdhGetRawCounterValue() fails: %s",
          mdr_exception_t::PdhErrorMessage(status));

  size = counter_type & SIZE_MASK;
  usage = counter_type & USAGE_MASK;

  // default to unknown value
  rval.mv_type = MVT_BOOL;

  switch(usage) {
  case PERF_TYPE_NUMBER:
    number = counter_type & NUMBER_MASK;
    switch(number) {
    case PERF_NUMBER_HEX:
    case PERF_NUMBER_DECIMAL:
    case PERF_NUMBER_DEC_1000:
      if (size == PERF_SIZE_DWORD)
        rval.mv_type = MVT_I32;
      else
        rval.mv_type = MVT_I64;
      break;
    }
    break;
  case PERF_TYPE_COUNTER:
    counter = counter_type & COUNTER_MASK;
    switch(counter) {
    case PERF_COUNTER_VALUE:
    case PERF_COUNTER_BASE:
    case PERF_COUNTER_ELAPSED:
    case PERF_COUNTER_QUEUELEN:
      if (size == PERF_SIZE_DWORD)
        rval.mv_type = MVT_I32;
      else
        rval.mv_type = MVT_I64;
      break;
    case PERF_COUNTER_RATE:
      is_rate = true;
    case PERF_COUNTER_FRACTION:
    case PERF_COUNTER_HISTOGRAM:
    case PERF_COUNTER_PRECISION:
      rval.mv_type = MVT_DOUBLE;
      break;
    }
    break;
  case PERF_TYPE_TEXT:
    text = counter_type & TEXT_MASK;
    switch(text) {
    case PERF_TEXT_UNICODE:
      // flag as unicode
      rval.mv_type = MVT_LONG;
      break;
    case PERF_TEXT_ASCII:
      rval.mv_type = MVT_STRING;
      break;
    }
    break;
  }

  switch(rval.mv_type) {
  case MVT_I32:
    status = get_counter_value(counter_handle,
                PDH_FMT_LONG | PDH_FMT_NOSCALE,
                &counter_type, &formatted_value, is_rate);
    if (status != ERROR_SUCCESS)
      format_error(
        "reg_query_t::get_counter_data: "
          "PdhGetFormattedCounterValue(I32) fails: %s",
            mdr_exception_t::PdhErrorMessage(status));

    rval.mv_i32 = (int) ((double) formatted_value.longValue);
    break;
  case MVT_I64:
    status = get_counter_value(counter_handle,
                PDH_FMT_LARGE | PDH_FMT_NOSCALE,
                &counter_type, &formatted_value, is_rate);
    if (status != ERROR_SUCCESS)
      format_error(
        "reg_query_t::get_counter_data: "
          "PdhGetFormattedCounterValue(I64) fails: %s",
            mdr_exception_t::PdhErrorMessage(status));

    rval.mv_i64 = (int64_t)
      ((double) formatted_value.largeValue);
    break;
  case MVT_DOUBLE:
    status = get_counter_value(counter_handle,
                PDH_FMT_DOUBLE | PDH_FMT_NOSCALE,
                &counter_type, &formatted_value, is_rate);
    if (status != ERROR_SUCCESS)
      format_error(
        "reg_query_t::get_counter_data: "
          "PdhGetFormattedCounterValue(DOUBLE) fails: %s",
            mdr_exception_t::PdhErrorMessage(status));

    rval.mv_d = formatted_value.doubleValue;
    break;
  case MVT_STRING:
    status = get_counter_value(counter_handle,
               PDH_FMT_ANSI, &counter_type, &formatted_value, is_rate);
    if (status != ERROR_SUCCESS)
      format_error(
        "reg_query_t::get_counter_data: "
          "PdhGetFormattedCounterValue(STRING) fails: %s",
            mdr_exception_t::PdhErrorMessage(status));

    if (rval.mv_s == 0)
      rval.mv_s = new std::string;
    *rval.mv_s = formatted_value.AnsiStringValue;
    break;
  case MVT_LONG:
    status = get_counter_value(counter_handle,
               PDH_FMT_UNICODE, &counter_type, &formatted_value, is_rate);
    if (status != ERROR_SUCCESS)
      format_error(
        "reg_query_t::get_counter_data: "
          "PdhGetFormattedCounterValue(LONG) fails: %s",
            mdr_exception_t::PdhErrorMessage(status));

    char tmp[256];

    _snprintf_s(tmp, sizeof tmp, sizeof tmp, "%S", formatted_value.WideStringValue);
    if (rval.mv_s == 0)
      rval.mv_s = new std::string;
    *rval.mv_s = tmp;
    rval.mv_type = MVT_STRING;
    break;
  case MVT_BOOL:
    // unsupported value
    rval.mv_type = MVT_I32;
    formatted_value.longValue = -249;
    break;
  }
}

}
