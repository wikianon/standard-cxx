#ifndef _MDR_H_
#define _MDR_H_

#include <Pdh.h>
#include <map>
#include <vector>
#include <string>
#include "mdr_value.h"
#include "mdr_exception.h"
#include "mdr_export.h"

namespace mdr {

// a map of int == string for the fields that are in classes that
// inherit mdr_t. The int is a const value in the individual class
// include files that represent the fields of the subclass.

typedef std::map<int, std::string> mdr_names_t;
typedef mdr_names_t::iterator mdr_names_node_t;

// a set of keys for tuples
typedef std::vector<std::string> mdr_keyset_t;
typedef mdr_keyset_t::iterator mdr_key_t;

// mapping of en_us to local and vice versa
typedef std::map<std::string, std::string> mdr_babel_t;
typedef mdr_babel_t::iterator mdr_babel_node_t;

// map names of categories and fields
class mdr_locale_t {
public:
  bool is_en_us_locale;
  bool has_been_converted;

  mdr_locale_t();
};

extern mdr_babel_t  win_to_utf8;     // windows-1252 to utf-8
extern mdr_babel_t  utf8_to_win;     // utf-8 to windows-1252
extern mdr_locale_t *mdr_locale;

class JMDR_API mdr_host_name_t : public std::string {
public:
  mdr_host_name_t();
};

class JMDR_API mdr_t {
public:
  static mdr_host_name_t mdr_local_host;  // name of this host

  std::string mdr_remote_host;        // Registry host
  std::string mdr_category_name;      // Name of this category.
  bool        mdr_is_singleton;       // Is this single or a tuple?
  int         mdr_field_count;        // Number of fields in this category.
  std::string mdr_explanation_text;   // Explanation of the category

  mdr_t(const char *name, bool singleton, const char *reg_host);
  virtual ~mdr_t() { }

  std::string &mdr_category_explanation(void)
  {
    return mdr_explanation_text;
  }
  const std::string &mdr_field_name(int field_number);
  virtual const std::string &mdr_field_explanation(int field_number, std::string &explanation) = 0;
  const std::string &mdr_field_explanation(const char *field_name, std::string &explanation)
  {
    return mdr_field_explanation(mdr_field_name_to_number(field_name), explanation);
  }

  virtual const mdr_value_t &mdr_field_value(int field_number)
  {
    throw mdr_exception_t("mdr_t::mdr_field_value: object requires a key");
  }

  virtual const mdr_value_t &mdr_field_value(const std::string &key,
                                             int field_number)
  {
    throw mdr_exception_t("mdr_t::mdr_field_value: object is a singleton");
  }

  virtual const mdr_keyset_t &mdr_keyset(mdr_keyset_t &name_set)
  {
    throw mdr_exception_t("mdr_t::mdr_keyset: object is a singleton");
  }

  int mdr_field_name_to_number(const char *name);
  int mdr_field_name_to_number(std::string &name)
  {
    return mdr_field_name_to_number(name.c_str());
  }

  mdr_keyset_t &mdr_all_field_names(mdr_keyset_t &name_set);
  mdr_keyset_t &mdr_supported_field_names(mdr_keyset_t &name_set)
  {
    // Windows: if it's there, then it's supported
    return mdr_all_field_names(name_set);
  }

  // updating
  virtual void mdr_refresh(void) = 0;
  void mdr_rates(void)
  {
    // no rates on Windows
  }
  void mdr_refresh_rates(void)
  {
    mdr_refresh();
    // no rates on Windows
  }

  static void   mdr_get_reg_instance_names(
    mdr_keyset_t &name_set, const char *category_name, const char *reg_host = 0);
  static mdr_t *mdr_get_mdr_object(
    const char *category_name, const char *reg_host = 0);

protected:
  mdr_names_t mdr_field_names;        // Text names of the fields.

  const std::string &get_explain_text(
    PDH_HCOUNTER counter_handle, std::string &explanation);
  void get_category_explanation(void);
};

extern std::ostream &operator<<(std::ostream &out, const mdr_value_t &v);

}

#endif
