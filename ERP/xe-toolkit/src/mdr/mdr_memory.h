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


#ifndef _MDR_MEMORY_H_
#define _MDR_MEMORY_H_

#include "mdr.h"

namespace mdr {

// version 1
const int MMEM_VERSION = 2;

const int MMEM_PAGED_IN_K     = 0;
const int MMEM_PAGED_OUT_K    = 1;
const int MMEM_PAGES_SCANNED  = 2;
const int MMEM_FREE_MEMORY_K  = 3;
const int MMEM_MEMORY_FREED_K = 4;
const int MMEM_PAGE_FAULTS    = 5;
const int MMEM_MAJOR_FAULTS   = 6;
const int MMEM_FIELD_COUNT    = 7;

class mdr_memory_t : public mdr_t {
public:
  // rates
  uint_t  mmem_paged_in_K;           // KBytes of memory paged in
  uint_t  mmem_paged_out_K;          // KBytes of memory paged out
  uint_t  mmem_pages_scanned;        // Number of pages scanned
  uint_t  mmem_page_faults;          // Number of page faults
  uint_t  mmem_major_faults;         // Number of major page faults

  // variants
  uint_t  mmem_free_memory_K;        // KBytes of free memory
  uint_t  mmem_memory_freed_K;       // KBytes of memory freed

  // constructors/destructors
  mdr_memory_t(const char *remote_host = 0);
 ~mdr_memory_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(key, field_number, value);
  }

private:
  uint_t old_paged_in_K;
  uint_t old_paged_out_K;
  uint_t old_pages_scanned;
  uint_t old_free_memory_K;
  uint_t old_page_faults;
  uint_t old_major_faults;

  void memory_remote_refresh(void);
};

} // end of namespace mdr

#endif
