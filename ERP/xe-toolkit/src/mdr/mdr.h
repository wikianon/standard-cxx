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


#ifndef _MDR_H_
#define _MDR_H_

#include <map>
#include <vector>
#include <string>
#include <libintl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include "mdr_types.h"
#include "mdr_value.h"
#include "mdr_client.h"
#include "mdr_exception.h"

namespace mdr {

// a map of int == string for the fields that are in classes that
// inherit mdr_t. The int is a const value in the individual class
// include files that represent the fields of the subclass.

typedef std::map<int, std::string> mdr_names_t;
typedef mdr_names_t::iterator mdr_names_node_t;

// a set of keys for tuples
typedef std::vector<std::string> mdr_keyset_t;
typedef mdr_keyset_t::iterator mdr_key_t;

extern const char *MDR_CACHE_NAME;
extern const char *MDR_CPU_NAME;
extern const char *MDR_CPUINFO_NAME;
extern const char *MDR_CPUSTAT_NAME;
extern const char *MDR_CTLR_NAME;
extern const char *MDR_CTLRSTAT_NAME;
extern const char *MDR_DISK_NAME;
extern const char *MDR_ICMP_NAME;
extern const char *MDR_IOSTAT_NAME;
extern const char *MDR_IP_NAME;
extern const char *MDR_MEMORY_NAME;
extern const char *MDR_MISC_NAME;
extern const char *MDR_MNT_NAME;
extern const char *MDR_MP_NAME;
extern const char *MDR_MPSTAT_NAME;
extern const char *MDR_NETIF_NAME;
extern const char *MDR_PF_NAME;
extern const char *MDR_PS_NAME;
extern const char *MDR_SYSTAT_NAME;
extern const char *MDR_TCP_NAME;
extern const char *MDR_THREAD_NAME;
extern const char *MDR_UDP_NAME;
extern const char *MDR_PROC_STATES_NAME;
extern const char *MDR_NETSTAT_NAME;

const char *const MDR_MSG_DOMAIN  = "mdr-messages";
const char *const MDR_EXP_DOMAIN  = "mdr-explanations";
const char *const MDR_NAME_DOMAIN = "mdr-names";

#define _GT_(T) dgettext(MDR_MSG_DOMAIN, T)
#define _GX_(T) dgettext(MDR_EXP_DOMAIN, T)
#define _GN_(T) dgettext(MDR_NAME_DOMAIN, T)

class os_version_t {
public:
  int os_version;
  int major_version;
  int minor_version;

  // only unix variants need addressing
  bool is_linux;
  bool is_solaris;
  bool is_darwin;
  bool is_freebsd;
  bool is_aix;
  bool is_hpux;

  os_version_t();
};

class mdr_t {
public:
  static os_version_t mdr_os_version; // per above

  std::string mdr_category_name;        // Name of this category.
  bool        mdr_category_supported;   // Perhaps this category is not supported.
  bool        mdr_is_singleton;         // Is this single or a tuple?
  int         mdr_field_count;          // Number of fields in this category.
  std::string mdr_explanation_text;     // Explanation of the category

  mdr_t(const char *name, bool supported, int field_count, bool singleton);
  virtual ~mdr_t() { }

  static std::string &mdr_localized_name(std::string &en_us_name, std::string &localized_name);
  static std::string &mdr_en_us_name(std::string &localized_name, std::string &en_us_name);
  void mdr_map_names(void);

  bool           mdr_is_supported(int field_number);
  std::string   &mdr_category_explanation(void)
  {
    return mdr_explanation_text;
  }
  std::string   &mdr_field_name(int field_number);
  std::string   &mdr_field_explanation(int field_number, std::string &explanation);
  std::string   &mdr_field_explanation(const char *field_name, std::string &explanation)
  {
    return mdr_field_explanation(mdr_field_name_to_number(field_name), explanation);
  }

  virtual const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value)
  {
    throw mdr_exception_t("mdr_t::mdr_field_value: %s", _GT_("object requires a key"));
  }

  virtual const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    throw mdr_exception_t("mdr_t::mdr_field_value: %s", _GT_("object is a singleton"));
  }

  virtual const mdr_keyset_t &mdr_keyset(mdr_keyset_t &keyset)
  {
    throw mdr_exception_t("mdr_t::mdr_keyset: %s", _GT_("object is a singleton"));
  }

  int mdr_field_name_to_number(const char *name);
  int mdr_field_name_to_number(std::string &name)
  {
    return mdr_field_name_to_number(name.c_str());
  }

  mdr_keyset_t &mdr_all_field_names(mdr_keyset_t &name_set);
  mdr_keyset_t &mdr_supported_field_names(mdr_keyset_t &name_set);

  // updating
  virtual void mdr_refresh(void) = 0;
  virtual void mdr_rates(void) = 0;
  void mdr_refresh_rates(void)
  {
    mdr_refresh();
    mdr_rates();
  }

  static void mdr_support_matrix(mdr_t &w, std::ostream &out);

protected:
  fd_set        mdr_support_mask;       // Whether fields are supported.
  mdr_names_t   mdr_field_names;        // Text names of the fields.
  mdr_client_t *net_client;

  uint_t wrap_diff_32(uint_t a, uint_t b)
  {
    if (a >= b)
      return a - b;

    return (uint_t) (((double) a + 4294967296.0) - b);
  }

  void mdr_support_field(int field_number);
  void mdr_unsupport_field(int field_number);
};

}

#endif
