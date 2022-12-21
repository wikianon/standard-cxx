#ifndef _MDR_VALUE_H_
#define _MDR_VALUE_H_

#include <iostream>

namespace mdr {

typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long ulong_t;

/// for the union below
enum mdr_value_type_t {
  MVT_I32,  MVT_UI32,  MVT_I64,    MVT_UI64,
  MVT_LONG, MVT_ULONG, MVT_DOUBLE, MVT_STRING,
  MVT_BOOL
};

// for mdr_field_value to return
class mdr_value_t {
public:
  mdr_value_type_t mv_type;

  // a union of the possible types for a value
  union {
    int          i32;
    uint32_t     ui32;
    int64_t      i64;
    uint64_t     ui64;
    long         l;
    ulong_t      ul;
    double       d;
    std::string *s;
    bool         b;
  } mv_un;
#define mv_i32  mv_un.i32
#define mv_ui32 mv_un.ui32
#define mv_i64  mv_un.i64
#define mv_ui64 mv_un.ui64
#define mv_l    mv_un.l
#define mv_ul   mv_un.ul
#define mv_d    mv_un.d
#define mv_s    mv_un.s
#define mv_b    mv_un.b

  void clear(void)
  {
    mv_un.d = 0;
  }
};

extern std::ostream &operator<<(std::ostream &out, const mdr_value_t &v);

}

#endif
