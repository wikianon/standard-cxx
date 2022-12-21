#ifndef _MDR_EXCEPTION_H_
#define _MDR_EXCEPTION_H_

#include <cstdio>
#include <cstdarg>
#include <cerrno>
#include <cstring>
#include <string>
#include <exception>
#include <Pdh.h>
#include "mdr_export.h"

namespace mdr {

class JMDR_API mdr_exception_t : public std::exception {
private:
  std::string message;

public:
  mdr_exception_t(const char *fmt, ...)
  {
    va_list arg;
    char buf[BUFSIZ];
	char errbuf[BUFSIZ];

    va_start(arg, fmt);
    _vsnprintf_s(buf, sizeof buf, fmt, arg);
    message = buf;
    if (errno) {
      message += ": ";
      message += strerror_s(errbuf, sizeof errbuf, errno);
    }
    va_end(arg);
  }

  ~mdr_exception_t() throw()
  {
  }

  const char *what() const throw()
  {
    return message.c_str();
  }

  static const char *PdhErrorMessage(PDH_STATUS status);
};

class JMDR_API pdh_exception_t : public std::exception {
private:
  std::string message;

public:
  pdh_exception_t(const char *fmt, ...)
  {
    va_list arg;
    char buf[BUFSIZ];
	char errbuf[BUFSIZ];

    va_start(arg, fmt);
    _vsnprintf_s(buf, sizeof buf, fmt, arg);
    message = buf;
    if (errno) {
      message += ": ";
      message += strerror_s(errbuf, sizeof errbuf, errno);
    }
    va_end(arg);
  }

  ~pdh_exception_t() throw()
  {
  }

  const char *what() const throw()
  {
    return message.c_str();
  }
};

}

#endif
