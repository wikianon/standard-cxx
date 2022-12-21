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


#ifndef _MDR_CATEGORY_H_
#define _MDR_CATEGORY_H_

#include <string>
#include <map>
#include <pthread.h>
#include "mdr.h"

namespace mdr {

// map of [class_name = mdr_t *]
typedef std::map<std::string, mdr_t *> mdr_cat_instance_map_t;
typedef mdr_cat_instance_map_t::iterator mdr_cat_instance_node_t;

// map of [host_name = mdr_cat_instance_map_t]
typedef std::map<std::string, mdr_cat_instance_map_t> mdr_host_instance_map_t;
typedef mdr_host_instance_map_t::iterator mdr_host_instance_node_t;

typedef std::map<mdr_t *, uint64_t>        mdr_cat_refresh_times_t;
typedef mdr_cat_refresh_times_t::iterator  mdr_cat_refresh_node_t;

class mdr_category_t {
public:
  std::string mdr_category_name;      // Name of this category.
  bool        mdr_category_supported; // Perhaps this category is not supported.
  bool        mdr_is_singleton;       // Is this single or a tuple?
  int         mdr_field_count;        // Number of fields in this category.

  mdr_category_t(const char *name, const char *hostname = 0);

  bool           mdr_is_supported(int field_number);
  std::string   &mdr_field_name(int field_number);
  std::string   &mdr_field_explanation(int field_number, std::string &explanation);
  std::string   &mdr_field_explanation(const char *field_name, std::string &explanation)
  {
    return mdr_field_explanation(mdr_field_name_to_number(field_name), explanation);
  }
  std::string  &mdr_category_explanation(void)
  {
    return mdr_obj->mdr_category_explanation();
  }

  // (int)
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value);

  // (std::string, std::string)
  const mdr_value_t &mdr_field_value(const std::string &key, const std::string &name, mdr_value_t &value)
  {
    return mdr_field_value(key.c_str(), mdr_field_name_to_number(name.c_str()), value);
  }

  // (char *, std::string)
  const mdr_value_t &mdr_field_value(const char *key, const std::string &name, mdr_value_t &value)
  {
    return mdr_field_value(key, mdr_field_name_to_number(name.c_str()), value);
  }

  // (char *, char *)
  const mdr_value_t &mdr_field_value(const char *key, const char *name, mdr_value_t &value)
  {
    return mdr_field_value(key, mdr_field_name_to_number(name), value);
  }

  // (std::string, char *)
  const mdr_value_t &mdr_field_value(const std::string &key, const char *name, mdr_value_t &value)
  {
    return mdr_field_value(key.c_str(), mdr_field_name_to_number(name), value);
  }

  // (int, std::string)
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    return mdr_field_value(key.c_str(), field_number, value);
  }

  // (int, char *)
  const mdr_value_t &mdr_field_value(const char *key, int field_number, mdr_value_t &value);

  int mdr_field_name_to_number(const char *name);
  int mdr_field_name_to_number(std::string &name)
  {
    return mdr_field_name_to_number(name.c_str());
  }

  mdr_keyset_t &mdr_all_field_names(mdr_keyset_t &keyset);
  mdr_keyset_t &mdr_supported_field_names(mdr_keyset_t &keyset);

  const mdr_keyset_t &mdr_get_instance_names(mdr_keyset_t &keyset);

  // updating
  bool mdr_refresh(void);
  void mdr_rates(void);
  void mdr_refresh_rates(void)
  {
    if (mdr_refresh())
      mdr_rates();
  }

  static void mdr_release_host(const char *hostname = 0);

protected:
  const char             *remote_host;
  mdr_t                  *mdr_obj;
  mdr_cat_instance_map_t *cat_inst_map;

  static mdr_host_instance_map_t cats_per_host;
  static pthread_mutex_t         cats_mux;
  static mdr_cat_refresh_times_t cat_times_map;
  static pthread_mutex_t         cat_times_mux;
};

}

#endif
