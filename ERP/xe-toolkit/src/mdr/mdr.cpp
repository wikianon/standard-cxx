/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr library of Captivity.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE-GPL.txt contained within the
** same package as this file. This software is subject to a
** dual-licensing mechanism, the details of which are outlined in
** file LICENSE-DUAL.txt, also contained within this package. Be sure
** to use the correct license for your needs. To view the commercial
** license, read LICENSE-COMMERCIAL.txt also contained within this
** package.
**
** If you do not have access to these files or are unsure which license
** is appropriate for your use, please contact the sales department at
** sales@captivemetrics.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#include <iostream>
#include <iomanip>
#include <locale.h>
#include <sys/utsname.h>
#include "mdr.h"

namespace mdr {

// <localized, en_us> names of categories and fields
typedef std::map<std::string, std::string> mdr_babel_t;
typedef mdr_babel_t::iterator mdr_babel_node_t;

// name mapping
static mdr_babel_t     mdr_names_map;
static pthread_mutex_t mdr_names_lock = PTHREAD_MUTEX_INITIALIZER;

const char *MDR_CACHE_NAME       = "Caches";
const char *MDR_CPU_NAME         = "CPU Utilization Data";
const char *MDR_CPUINFO_NAME     = "CPU Information";
const char *MDR_CPUSTAT_NAME     = "Global CPU Metrics";
const char *MDR_CTLR_NAME        = "Controllers";
const char *MDR_CTLRSTAT_NAME    = "Controller Metrics";
const char *MDR_DISK_NAME        = "Disks";
const char *MDR_ICMP_NAME        = "ICMP Protocol";
const char *MDR_IOSTAT_NAME      = "Disk Metrics";
const char *MDR_IP_NAME          = "IP Protocol";
const char *MDR_MEMORY_NAME      = "Memory Data";
const char *MDR_MISC_NAME        = "Miscellaneous Data";
const char *MDR_MNT_NAME         = "Mount Points";
const char *MDR_MP_NAME          = "Per-CPU Data";
const char *MDR_MPSTAT_NAME      = "Per-CPU Metrics";
const char *MDR_NETIF_NAME       = "Network Interfaces";
const char *MDR_PF_NAME          = "Paging Files";
const char *MDR_PS_NAME          = "Processes";
const char *MDR_SYSTAT_NAME      = "System Metrics";
const char *MDR_TCP_NAME         = "TCP Protocol";
const char *MDR_THREAD_NAME      = "Threads";
const char *MDR_UDP_NAME         = "UDP Protocol";
const char *MDR_PROC_STATES_NAME = "Process States";
const char *MDR_NETSTAT_NAME     = "Interface Metrics";

static const char **all_categories[] = {
    &MDR_CACHE_NAME,
    &MDR_CPU_NAME,
    &MDR_CPUINFO_NAME,
    &MDR_CPUSTAT_NAME,
    &MDR_CTLR_NAME,
    &MDR_CTLRSTAT_NAME,
    &MDR_DISK_NAME,
    &MDR_ICMP_NAME,
    &MDR_IOSTAT_NAME,
    &MDR_IP_NAME,
    &MDR_MEMORY_NAME,
    &MDR_MISC_NAME,
    &MDR_MNT_NAME,
    &MDR_MP_NAME,
    &MDR_MPSTAT_NAME,
    &MDR_NETIF_NAME,
    &MDR_PF_NAME,
    &MDR_PS_NAME,
    &MDR_SYSTAT_NAME,
    &MDR_TCP_NAME,
    &MDR_THREAD_NAME,
    &MDR_UDP_NAME,
    &MDR_PROC_STATES_NAME
};

class mdr_locale_t {
public:
  bool en_us_locale;

  mdr_locale_t() {
    const char *locale_dir = getenv("LOCALEDIR");
    int i;
    int n = sizeof(all_categories) / sizeof(char **);

#ifdef _LOCALE_DEBUG_
    std::cout << "locale_dir: " << (locale_dir ? locale_dir : "null")
              << std::endl;
#endif

    if (locale_dir == 0) {
      locale_dir = "/CM/product/locale";
#ifdef _LOCALE_DEBUG_
      std::cout << "locale_dir: " << locale_dir << std::endl;
#endif
    }

    // initialize locale code from environment
    setlocale(LC_MESSAGES, "");

    // retrieve the current locale for LC_MESSAGES
    const char *lm = setlocale(LC_MESSAGES, 0);

#ifdef _LOCALE_DEBUG_
    std::cout << "LC_MESSAGES: " << lm << std::endl;
#endif

    // if it's null or set to C, default to en_US.UTF-8
    if (!lm || strcmp(lm, "C") == 0) {
#ifdef _AIX
        lm = "en_US";
#else
        lm = "en_US.UTF-8";
#endif
#ifdef _LOCALE_DEBUG_
        std::cout << "setting lm to: " << lm << std::endl;
#endif
    }

    en_us_locale = (strncmp(lm, "en_US", 5) == 0);
#ifdef _LOCALE_DEBUG_
    std::cout << "is en_US locale?: " << (en_us_locale ? "true" : "false")
              << std::endl;
#endif

    // set the locale as such
    if (setlocale(LC_MESSAGES, lm) == 0) {
      std::cerr << "cannot setlocale" << std::endl;
      exit(1);
    }

    // bind the following domains to the locale dir
    if (bindtextdomain(MDR_MSG_DOMAIN,  locale_dir) == 0 ||
        bindtextdomain(MDR_EXP_DOMAIN,  locale_dir) == 0 ||
        bindtextdomain(MDR_NAME_DOMAIN, locale_dir) == 0) {
      std::cerr << "cannot bindtextdomain" << std::endl;
      exit(1);
    }

    // map all category names and save back to global
    pthread_mutex_lock(&mdr_names_lock);
      for(i=0; i<n; i++) {
        const char *local_cat = _GN_(*all_categories[i]);
        mdr_names_map[local_cat] = *all_categories[i];
        *all_categories[i] = local_cat;
      }
    pthread_mutex_unlock(&mdr_names_lock);
  }
};

// cause the C++ runtime to set the locale before calling main
static mdr_locale_t mdr_locale;

// more static initialization magic, but available to all mdr subclasses
os_version_t mdr_t::mdr_os_version;

os_version_t::os_version_t()
{
  struct utsname u;
  char *p;

  uname(&u);
#ifdef _AIX
  major_version = atoi(u.version);
  minor_version = atoi(u.release);
#else
  major_version = atoi(u.release);
  p = strchr(u.release, '.');
  if (p == 0)
    minor_version = 0;
  else
    minor_version = atoi(p+1);
#endif
  os_version = (major_version * 100) + (minor_version * 10);

  is_linux =
  is_solaris =
  is_darwin =
  is_freebsd =
  is_aix =
  is_hpux = false;

  if (strcmp(u.sysname, "Linux") == 0)
    is_linux = true;
  else if (strcmp(u.sysname, "SunOS") == 0)
    is_solaris = true;
  else if (strcmp(u.sysname, "Darwin") == 0)
    is_darwin = true;
  else if (strcmp(u.sysname, "FreeBSD") == 0)
    is_freebsd = true;
  else if (strcmp(u.sysname, "AIX") == 0)
    is_aix = true;
  else if (strcmp(u.sysname, "HP-UX") == 0)
    is_hpux = true;
}

mdr_t::mdr_t(const char *name, bool supported, int field_count, bool singleton)
{
  mdr_category_name = name;
  mdr_category_supported = supported;
  mdr_field_count = field_count;
  FD_ZERO(&mdr_support_mask);
  mdr_is_singleton = singleton;
  net_client = 0;
}

void mdr_t::mdr_map_names(void)
{
  // no mapping of en_us to en_us
  if (mdr_locale.en_us_locale)
    return;

  mdr_names_node_t node;

  // lock the map once
  pthread_mutex_lock(&mdr_names_lock);

  // for each field name
  for(node=mdr_field_names.begin(); node != mdr_field_names.end(); node++) {
    // the en_us name
    const char *en_us = node->second.c_str();

    // lookup the localized name
    const char *lname = _GN_(en_us);

    // if they're not the same
    if (strcmp(lname, en_us)) {
      // map the localized to the en_us
      mdr_names_map[lname] = en_us;

      // change mdr_field_names to the localized version
      node->second = lname;
    }
  }

  // done. unlock the map
  pthread_mutex_unlock(&mdr_names_lock);
}

bool mdr_t::mdr_is_supported(int field_number)
{
  if (FD_ISSET(field_number, &mdr_support_mask))
    return true;

  return false;
}

void mdr_t::mdr_support_field(int field_number)
{
  FD_SET(field_number, &mdr_support_mask);
}

void mdr_t::mdr_unsupport_field(int field_number)
{
  FD_CLR(field_number, &mdr_support_mask);
}

std::string &mdr_t::mdr_field_name(int field_number)
{
  mdr_names_node_t n = mdr_field_names.find(field_number);
  static std::string not_found(_GT_("FIELD NOT FOUND"));

  if (n == mdr_field_names.end())
    return not_found;

  return n->second;
}

std::string &mdr_t::mdr_field_explanation(int field_number, std::string &explanation)
{
  mdr_names_node_t n = mdr_field_names.find(field_number);
  static std::string not_found(_GT_("FIELD NOT FOUND"));

  if (n == mdr_field_names.end())
    return not_found;

  // XXX: this is not a correct solution for remote objects
  explanation = _GX_(n->second.c_str());
  return explanation;
}

int mdr_t::mdr_field_name_to_number(const char *name)
{
  mdr_names_node_t n;

  // oogie<>linear search...
  for(n=mdr_field_names.begin(); n != mdr_field_names.end(); n++)
    if (n->second == name)
      return n->first;

  return -1;
}

mdr_keyset_t &mdr_t::mdr_all_field_names(mdr_keyset_t &name_set)
{
  mdr_names_node_t n;
  int i;

  name_set.clear();
  name_set.resize(mdr_field_names.size());

  for(i=0, n=mdr_field_names.begin(); n != mdr_field_names.end(); n++, i++)
    name_set[i] = n->second;

  return name_set;
}

mdr_keyset_t &mdr_t::mdr_supported_field_names(mdr_keyset_t &name_set)
{
  mdr_names_node_t n;
  int i;
  int count = 0;

  name_set.clear();

  for(i=0; i<mdr_field_count; i++)
    if (mdr_is_supported(i))
      count++;

  name_set.resize(count);
  count = 0;

  for(i=0, n=mdr_field_names.begin(); n != mdr_field_names.end(); n++, i++)
    if (mdr_is_supported(i))
      name_set[count++] = n->second;

  return name_set;
}

std::ostream &operator<<(std::ostream &out, const mdr_value_t &v)
{
  switch(v.mv_type) {
  case MVT_I32:
    out << v.mv_i32;
    break;
  case MVT_UI32:
    out << v.mv_ui32;
    break;
  case MVT_I64:
    out << v.mv_i64;
    break;
  case MVT_UI64:
    out << v.mv_ui64;
    break;
  case MVT_LONG:
    out << v.mv_l;
    break;
  case MVT_ULONG:
    out << v.mv_ul;
    break;
  case MVT_DOUBLE:
    out << v.mv_d;
    break;
  case MVT_STRING:
    if (v.mv_s == 0)
      out << "null";
    else
      out << *v.mv_s;
    break;
  case MVT_BOOL:
    out << (v.mv_b ? _GT_("true") : _GT_("false"));
    break;
  }

  return out;
}

void mdr_t::mdr_support_matrix(mdr_t &m, std::ostream &out)
{
  int i;
  int n;
  int len = 0;

  for(i=0; i<m.mdr_field_count; i++) {
    n = m.mdr_field_name(i).length();
    if (n > len)
      len = n;
  }

  len++;

  out << _GT_("Category : ") << m.mdr_category_name << std::endl;
  out << _GT_("Singleton: ") << (m.mdr_is_singleton ? _GT_("yes") : _GT_("no"))
      << std::endl;
  out << _GT_("Supported: ") << (m.mdr_category_supported ? _GT_("yes") : _GT_("no"))
      << std::endl << std::endl;

  if (!m.mdr_category_supported)
    return;

  out << std::setw(len) << std::left << _GT_("Description")
      << std::setw(0)   << _GT_("Supported?") << std::endl << std::endl;

  for(i=0; i<m.mdr_field_count; i++)
    out << std::setw(len) << std::left << m.mdr_field_name(i)
        << std::setw(0)   << (m.mdr_is_supported(i) ? _GT_("yes") : _GT_("no"))
        << std::endl;
}

std::string &mdr_t::mdr_localized_name(
  std::string &en_us_name, std::string &localized_name)
{
  // no mapping of en_us to en_us
  if (mdr_locale.en_us_locale) {
    localized_name = en_us_name;
    return localized_name;
  }

  const char *us = en_us_name.c_str();
  const char *lname = _GN_(us);

  // if it's not there
  if (strcmp(lname, us) == 0) {
    localized_name = en_us_name;
    return localized_name;
  }

  // put it in the names map if not there
  pthread_mutex_lock(&mdr_names_lock);
    if (mdr_names_map.find(lname) == mdr_names_map.end())
      mdr_names_map[lname] = us;
  pthread_mutex_unlock(&mdr_names_lock);

  localized_name = lname;
  return localized_name;
}

std::string &mdr_t::mdr_en_us_name(std::string &localized_name, std::string &en_us_name)
{
  // cannot lookup the en_us version of an en_us string
  if (mdr_locale.en_us_locale) {
    en_us_name = localized_name;
    return en_us_name;
  }

  mdr_babel_node_t node;

  pthread_mutex_lock(&mdr_names_lock);
    node = mdr_names_map.find(localized_name);
    if (node == mdr_names_map.end())
      en_us_name = localized_name;
    else
      en_us_name = node->second;
  pthread_mutex_unlock(&mdr_names_lock);

  return en_us_name;
}

}
