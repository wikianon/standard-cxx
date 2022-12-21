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


#ifndef _MDR_MAPS_H_
#define _MDR_MAPS_H_

#include <set>
#include <map>
#include <math.h>
#include "mdr_types.h"

namespace mdr {

////////////////////////////////////////////////////////////////////
// line_token code
////////////////////////////////////////////////////////////////////

class line_token {
private:
  char *value;

public:
  line_token()
  {
    value = 0;
  }

  line_token(char *new_value)
  {
    value = new_value;
  }

  void assign(char *new_value)
  {
    value = new_value;
  }

  int32_t to_int32(void)
  {
    if (value == 0)
      return -1;
    return atoi(value);
  }

  uint32_t to_uint32(void)
  {
    if (value == 0)
      return 0xffffffff;
    return (uint32_t) strtoul(value, 0, 0);
  }

  int64_t to_int64()
  {
    if (value == 0)
      return -1;
    return strtoll(value, 0, 0);
  }

  uint64_t to_uint64(void)
  {
    if (value == 0)
      return 0xffffffffffffffffULL;
    return strtoull(value, 0, 0);
  }

  double to_double()
  {
    if (value == 0)
      return -1;
    return atof(value);
  }

  double to_ceil()
  {
    double d;

    if (value == 0)
      return -1;
    d = atof(value);
    return ceil(d);
  }

  long to_long(void)
  {
    if (value == 0)
      return -1;
    return atol(value);
  }

  ulong_t to_ulong(void)
  {
    if (value == 0)
      return 0xffffffff;
    return strtoul(value, 0, 0);
  }

  char *token()
  {
    return value;
  }
};

////////////////////////////////////////////////////////////////////
// line_map code
////////////////////////////////////////////////////////////////////

class line_map : public std::map<int, line_token> {
private:
  char *local_storage;

public:
  int token_count;

  line_map()
  {
    local_storage = 0;
    token_count = 0;
  }

  line_map(char *line, const char *delim)
  {
    assign(line, delim);
  }

 ~line_map()
  {
    if (local_storage)
      free(local_storage);
  }

  void assign(char *line, const char *delim);
};

////////////////////////////////////////////////////////////////////
// file_map code
////////////////////////////////////////////////////////////////////

class file_map : public std::map<int, line_map> {
private:
  bool file_exists;

  void assign(const char *file_name, const char *delim);

public:
  int line_count;

  file_map(const char *file_name);
  file_map(const char *file_name, const char *delim);

  // moron
  line_map &operator[](int index)
  {
    return std::map<int, line_map>::operator[](index);
  }

  line_map &operator[](const char *token0);

  line_map &find(const char *token0, int instance);

  bool operator!()
  {
    return !file_exists;
  }

  bool operator()()
  {
    return file_exists;
  }
};

////////////////////////////////////////////////////////////////////
// dir_list_t code
////////////////////////////////////////////////////////////////////

class dir_list_t : public std::set<std::string> {
public:
  dir_list_t(const char *dir_name, bool include_dots = false);

  bool operator!(void);
  bool operator()(void);

private:
  bool open_succeeded;
};

typedef dir_list_t::iterator dir_node_t;

} // namespace mdr

#endif
