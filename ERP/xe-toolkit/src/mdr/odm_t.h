#ifndef _ODM_T_H_
#define _ODM_T_H_

#include <map>
#include <odmi.h>
#include "mdr_exception.h"

namespace mdr {

template <typename odm_type>
class odm_t : public std::map<int, odm_type *> {
private:
  int node_count;

public:
  odm_t(Class odm_class[], char *criteria = 0) {
    CLASS_SYMBOL cd;
    odm_type obj;
    odm_type *node;

    if (odm_initialize() == -1)
      throw mdr_exception_t("odm_t::odm_t: odm_initialize fails");

    cd = odm_open_class(odm_class);
    if (cd == (CLASS_SYMBOL) -1)
      throw mdr_exception_t("odm_t::odm_t: odm_open_class fails");

    node_count = 0;
    for(node=(odm_type *) odm_get_first(cd, criteria, &obj); node != 0;
        node=(odm_type *) odm_get_next(cd, &obj)) {
      odm_type *new_node = new odm_type;
      memcpy(new_node, node, sizeof(odm_type));
      (*this)[node_count++] = new_node;
    }

    odm_close_class(cd);
    odm_terminate();
  }

  ~odm_t() {
    int n = size();

    for(int i=0; i<n; i++)
      delete (*this)[i];
  }
};

}

#endif
