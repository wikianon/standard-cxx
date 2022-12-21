#ifndef _MDR_CATEGORY_H_
#define _MDR_CATEGORY_H_

#include <windows.h>
#include <string>
#include <map>
#include "mdr.h"
#include "mdr_export.h"

namespace mdr {

// map of [class_name = mdr_t *]
typedef std::map<std::string, mdr_t *> mdr_cat_instance_map_t;
typedef mdr_cat_instance_map_t::iterator mdr_cat_instance_node_t;

// map of [host_name = mdr_cat_instance_map_t]
typedef std::map<std::string, mdr_cat_instance_map_t> mdr_host_instance_map_t;
typedef mdr_host_instance_map_t::iterator mdr_host_instance_node_t;

typedef std::map<mdr_t *, DWORD>          mdr_cat_refresh_times_t;
typedef mdr_cat_refresh_times_t::iterator mdr_cat_refresh_node_t;

class JMDR_API mdr_category_t {
public:
  std::string mdr_category_name;      // Name of this category.
  bool        mdr_category_supported; // Perhaps this category is not supported.
  bool        mdr_is_singleton;       // Is this single or a tuple?
  int         mdr_field_count;        // Number of fields in this category.

  mdr_category_t(const char *name, const char *hostname = 0);

  const std::string   &mdr_field_name(int field_number);
  const std::string   &mdr_field_explanation(int field_number, std::string &explanation);
  const std::string   &mdr_field_explanation(const char *field_name, std::string &explanation)
  {
    return mdr_field_explanation(mdr_field_name_to_number(field_name), explanation);
  }
  std::string  &mdr_category_explanation(void)
  {
    return mdr_obj->mdr_category_explanation();
  }

  // (int)
  const mdr_value_t &mdr_field_value(int field_number);

  // (std::string, std::string)
  const mdr_value_t &mdr_field_value(const std::string &key, const std::string &name)
  {
    return mdr_field_value(key.c_str(), mdr_field_name_to_number(name.c_str()));
  }

  // (char *, std::string)
  const mdr_value_t &mdr_field_value(const char *key, const std::string &name)
  {
    return mdr_field_value(key, mdr_field_name_to_number(name.c_str()));
  }

  // (char *, char *)
  const mdr_value_t &mdr_field_value(const char *key, const char *name)
  {
    return mdr_field_value(key, mdr_field_name_to_number(name));
  }

  // (std::string, char *)
  const mdr_value_t &mdr_field_value(const std::string &key, const char *name)
  {
    return mdr_field_value(key.c_str(), mdr_field_name_to_number(name));
  }

  // (int, std::string)
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number)
  {
    return mdr_field_value(key.c_str(), field_number);
  }

  // (int, char *)
  const mdr_value_t &mdr_field_value(const char *key, int field_number);

  int mdr_field_name_to_number(const char *name);
  int mdr_field_name_to_number(std::string &name)
  {
    return mdr_field_name_to_number(name.c_str());
  }

  mdr_keyset_t &mdr_all_field_names(mdr_keyset_t &name_set);
  mdr_keyset_t &mdr_supported_field_names(mdr_keyset_t &name_set);

  const mdr_keyset_t &mdr_get_instance_names(mdr_keyset_t &name_set);

  // updating
  void mdr_refresh(void);
  void mdr_rates(void)
  {
    // no rates on Windows
  }
  void mdr_refresh_rates(void)
  {
    mdr_refresh();
    // no rates on Windows
  }

  static void mdr_release_host(const char *hostname = 0);

private:
  const char             *remote_host;
  mdr_t                  *mdr_obj;
  mdr_cat_instance_map_t *cat_inst_map;

  friend void mdr_category_init(void);
  static mdr_host_instance_map_t cats_per_host;
  static CRITICAL_SECTION        cats_mux;
  static mdr_cat_refresh_times_t cat_times_map;
  static CRITICAL_SECTION        cat_times_mux;
};

}

#endif
