#include <windows.h>
#include <Pdh.h>
#include <PdhMsg.h>
#include <jni.h>
#include "mdr_perf_host.h"
#include "mdr_category.h"

namespace mdr {

mdr_babel_t  win_to_utf8;     // windows-xxxx to utf-8
mdr_babel_t  utf8_to_win;     // utf-8 to windows-xxxx
mdr_locale_t *mdr_locale;

static mdr_babel_t  en_us_to_local;  // english to localized
static mdr_babel_t  local_to_en_us;  // localized to english

void mdr_perf_host_init_locale(void)
{
  if (mdr_locale == 0)
    mdr_locale = new mdr_locale_t();
}

mdr_locale_t::mdr_locale_t()
{
  char acp[128];
  _snprintf_s(acp, sizeof acp, sizeof acp, ".%d", GetACP());
  const char *locale = setlocale(LC_ALL, acp);

  is_en_us_locale = false;
  has_been_converted = false;
  if (locale &&
    ((strcmp(locale, "C") == 0) || (strncmp(locale, "English_USA", 11) == 0)))
      is_en_us_locale = true;

  // no mapping in English_USA.1252
  if (is_en_us_locale) {
    has_been_converted = true;
    return;
  }

  // ok, let's use some memory

  DWORD dwType;
  LPBYTE lpmszCounters = 0;
  DWORD dwStatus;
  DWORD dwSize = 0;
  const DWORD dwIncrement = 4096;
  mdr_babel_t l2e;
  mdr_babel_t e2l;

  do {
    if (lpmszCounters != 0)
      delete[] lpmszCounters;
    dwSize += dwIncrement;
    lpmszCounters = new BYTE[dwSize];
    dwStatus = RegQueryValueEx(
      HKEY_PERFORMANCE_DATA,
      "Counter 009", 
       NULL, 
       &dwType, 
       lpmszCounters,
       &dwSize);
  } while(dwStatus == ERROR_MORE_DATA);

  char lbuf[BUFSIZ];
  DWORD lbufsz;
  const char *cp;
  bool index = true;
  DWORD index_number;

  for (cp = (const char *) lpmszCounters; *cp; cp += strlen(cp) + 1) {
    if (index) {
      index_number = (DWORD) atoi(cp);
      index = false;
      continue;
    } else
      index = true;

    lbufsz = sizeof(lbuf);
    PdhLookupPerfNameByIndex(0, index_number, lbuf, &lbufsz);

    en_us_to_local[cp] = lbuf;
    local_to_en_us[lbuf] = cp;
  }

  delete[] lpmszCounters;
}

mdr_performance_host_t::mdr_performance_host_t(const char *host_name)
{
  if (host_name)
    remote_host = host_name;
  else
    remote_host = mdr_t::mdr_local_host.c_str();
}

const mdr_keyset_t &mdr_performance_host_t::mdr_get_category_names(void)
{
  PDH_STATUS status;
  LPTSTR counter_list = 0;
  DWORD counter_length = 0;
  LPTSTR instance_list = 0;
  DWORD instance_length = 0;
  LPTSTR object_list = 0;
  DWORD object_length = 0;
  const char *cat;
  int cat_count = 0;
  int i;
  const char *localhost = "localhost";

  status = PdhConnectMachine(remote_host);

  status = PdhEnumObjects(
    0, remote_host, object_list, &object_length,
      PERF_DETAIL_WIZARD, TRUE);
  if (status != ERROR_SUCCESS && status != PDH_MORE_DATA)
    throw mdr_exception_t(
      "mdr_performance_host_t::mdr_get_category_names: "
        "PdhEnumObjects(%s, size) failure: %s",
          remote_host, mdr_exception_t::PdhErrorMessage(status));

  object_list = new char[object_length];
  
  status = PdhEnumObjects(
    0, remote_host, object_list, &object_length,
      PERF_DETAIL_WIZARD, FALSE);
  if (status != ERROR_SUCCESS)
  {
    delete[] object_list;
    throw mdr_exception_t(
      "mdr_performance_host_t::mdr_get_category_names: "
        "PdhEnumObjects(%s, data) failure: %s",
          remote_host, mdr_exception_t::PdhErrorMessage(status));
  }

  // for each category name
  for(cat=object_list; *cat; cat += strlen(cat) + 1)
    cat_count++;

  category_names.clear();
  category_names.resize(cat_count);

  for(i=0, cat=object_list; *cat; cat += strlen(cat) + 1)
  {
    // reset to zip for each category
    counter_list = 0;
    counter_length = 0;
    instance_list = 0;
    instance_length = 0;

    // query its object items and see if it's a tuple with no instances
    status = PdhEnumObjectItems(
      0, remote_host, cat, counter_list, &counter_length,
        instance_list, &instance_length, PERF_DETAIL_WIZARD, 0);
    if (status != ERROR_SUCCESS && status != PDH_MORE_DATA)
      // bogus category name slipped into the registry
      continue;
#if 0
      throw mdr_exception_t(
        "mdr_performance_host_t::mdr_get_category_names: "
          "PdhEnumObjectItems(%s:size) failure: %s",
            cat, mdr_exception_t::PdhErrorMessage(status));
#endif

    // category with no instances; no dice
    if (instance_length == 2)
      continue;

    // translate to utf8 if necessary
    const char *name_to_insert = cat;
    mdr_babel_node_t n = win_to_utf8.find(cat);

    if (n != win_to_utf8.end())
      name_to_insert = n->second.c_str();

    category_names[i++] = name_to_insert;
  }

  category_names.resize(i);

  delete[] object_list;

  return category_names;
}

mdr_category_t *
  mdr_performance_host_t::mdr_get_category_by_name(const char *name)
{
  if (name == 0)
    throw mdr_exception_t(
      "mdr_performance_host_t::mdr_get_category_by_name: null name");

  return new mdr_category_t(name, remote_host);
}

std::string &mdr_performance_host_t::mdr_localized_name(
  std::string &en_us_name, std::string &localized_name)
{
  if (mdr_locale->is_en_us_locale) {
    localized_name = en_us_name;
    return localized_name;
  }

  mdr_babel_node_t node = en_us_to_local.find(en_us_name);
  if (node == en_us_to_local.end()) {
    localized_name = en_us_name;
    return localized_name;
  }

  localized_name = node->second;
  return localized_name;
}

std::string &mdr_performance_host_t::mdr_en_us_name(
  std::string &localized_name, std::string &en_us_name)
{
  if (mdr_locale->is_en_us_locale) {
    en_us_name = localized_name;
    return en_us_name;
  }

  mdr_babel_node_t node = local_to_en_us.find(localized_name);
  if (node == local_to_en_us.end()) {
    en_us_name = localized_name;
    return en_us_name;
  }

  en_us_name = node->second;
  return en_us_name;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1host_1t_convert_1strings_1to_1utf8(JNIEnv *env, jobject jobj, jstring jcallback)
{
  if (mdr_locale->is_en_us_locale || mdr_locale->has_been_converted)
    return;

  jclass cls = env->GetObjectClass(jobj);
  const char *callback = env->GetStringUTFChars(jcallback, 0);

  jmethodID mid = env->GetMethodID(cls, callback, "([Ljava/lang/String;)V");
  if (mid == 0) {
    env->ReleaseStringUTFChars(jcallback, callback);
    return;
  }

  // size of array passed to callback
  size_t n = en_us_to_local.size();

  // make the array
  jobjectArray jarray = env->NewObjectArray(
    n, env->FindClass("java/lang/String"), env->NewStringUTF(""));

  size_t i = 0;
  mdr_babel_node_t node;

  // populate the array with the localized strings
  for(node=en_us_to_local.begin(); node != en_us_to_local.end(); node++) {
    jstring jstr = env->NewStringUTF(node->second.c_str());
    env->SetObjectArrayElement(jarray, i++, jstr);
  }

  // call the callback to get the strings translated
  env->CallVoidMethod(jobj, mid, jarray);

  // this will be completely reconstructed
  local_to_en_us.clear();

  // put the strings back in
  for(i=0, node=en_us_to_local.begin(); node != en_us_to_local.end(); node++) {
    // get the object element
    jstring jstr = (jstring) env->GetObjectArrayElement(jarray, i);

    // this converts the string to UTF-8
    const char *str = env->GetStringUTFChars(jstr, 0);

    // map the windows name to the utf8 name and vice versa.
    win_to_utf8[node->second] = str;
    utf8_to_win[str] = node->second;

    node->second = str;
    i++;

    local_to_en_us[node->second] = node->first;
  }

  // The first mdr_performance_host_t in Javaland will convert the strings
  // from windows-xxxx (or whatever) to UTF-8. See matching comment in
  // src/java/CM/com/capmet/metrics/jmdr/mdr_performance_host_t.java.

  mdr_locale->has_been_converted = true;
}

}
