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


#include "mdr_memory.h"

namespace mdr {

mdr_memory_t::mdr_memory_t(const char *remote_host) :
  mdr_t(MDR_MEMORY_NAME, true, MMEM_FIELD_COUNT, true)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_memory_t", remote_host, MMEM_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MMEM_FREE_MEMORY_K);
    mdr_support_field(MMEM_PAGED_IN_K);
    mdr_support_field(MMEM_PAGED_OUT_K);
    if ((mdr_os_version.is_linux && (mdr_os_version.os_version > 240)) ||
       mdr_os_version.is_solaris || mdr_os_version.is_freebsd ||
       mdr_os_version.is_aix ||
       (mdr_os_version.is_darwin && (mdr_os_version.os_version < 830))) {
      mdr_support_field(MMEM_PAGES_SCANNED);
    }
    mdr_support_field(MMEM_MEMORY_FREED_K);
#if !defined(_AIX)
    mdr_support_field(MMEM_PAGE_FAULTS);
    mdr_support_field(MMEM_MAJOR_FAULTS);
#endif

    net_client = 0;
  }

  mdr_field_names[MMEM_FREE_MEMORY_K]  = "KBytes Free Memory";
  mdr_field_names[MMEM_PAGED_IN_K]     = "KBytes Memory Paged In";
  mdr_field_names[MMEM_PAGED_OUT_K]    = "KBytes Memory Paged Out";
  mdr_field_names[MMEM_PAGES_SCANNED]  = "Pages Scanned";
  mdr_field_names[MMEM_MEMORY_FREED_K] = "KBytes Memory Freed";
  mdr_field_names[MMEM_PAGE_FAULTS]    = "Total Page Faults";
  mdr_field_names[MMEM_MAJOR_FAULTS]   = "Major Page Faults";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Memory_Category");

  mmem_paged_in_K = 0;
  mmem_paged_out_K = 0;
  mmem_pages_scanned = 0;
  mmem_free_memory_K = 0;
  mmem_memory_freed_K = 0;
  mmem_page_faults = 0;
  mmem_major_faults = 0;

  old_paged_in_K = 0;
  old_paged_out_K = 0;
  old_pages_scanned = 0;
  old_free_memory_K = 0;

  mdr_refresh_rates();
}

mdr_memory_t::~mdr_memory_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_memory_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  value.mv_type = MVT_UI32;

  switch(field_number) {
  case MMEM_PAGED_IN_K:
    value.mv_ui32 = mmem_paged_in_K;
    break;
  case MMEM_PAGED_OUT_K:
    value.mv_ui32 = mmem_paged_out_K;
    break;
  case MMEM_PAGES_SCANNED:
    value.mv_ui32 = mmem_pages_scanned;
    break;
  case MMEM_FREE_MEMORY_K:
    value.mv_ui32 = mmem_free_memory_K;
    break;
  case MMEM_MEMORY_FREED_K:
    value.mv_ui32 = mmem_memory_freed_K;
    break;
  case MMEM_PAGE_FAULTS:
    value.mv_ui32 = mmem_page_faults;
    break;
  case MMEM_MAJOR_FAULTS:
    value.mv_ui32 = mmem_major_faults;
    break;
  }

  return value;
}

void mdr_memory_t::mdr_rates(void)
{
  uint_t tmp_paged_in_K = mmem_paged_in_K;
  uint_t tmp_paged_out_K = mmem_paged_out_K;
  uint_t tmp_pages_scanned = mmem_pages_scanned;
  uint_t tmp_page_faults = mmem_page_faults;
  uint_t tmp_major_faults = mmem_major_faults;

  mmem_paged_in_K    = wrap_diff_32(mmem_paged_in_K, old_paged_in_K);
  mmem_paged_out_K   = wrap_diff_32(mmem_paged_out_K, old_paged_out_K);
  mmem_pages_scanned = wrap_diff_32(mmem_pages_scanned, old_pages_scanned);
  mmem_page_faults   = wrap_diff_32(mmem_page_faults, old_page_faults);
  mmem_major_faults  = wrap_diff_32(mmem_major_faults, old_major_faults);

  old_paged_in_K    = tmp_paged_in_K;
  old_paged_out_K   = tmp_paged_out_K;
  old_pages_scanned = tmp_pages_scanned;
  old_page_faults   = tmp_page_faults;
  old_major_faults  = tmp_major_faults;
}

void mdr_memory_t::memory_remote_refresh(void)
{
  mdr_s_mm_t s_map;
  int i;
  int n;

  net_client->mc_refresh(s_map);
  for(i=0, n=s_map.size(); i<n; i++) {
    switch(i) {
    case MMEM_PAGED_IN_K:
      mmem_paged_in_K = s_map[i].mv_ui32;
      break;
    case MMEM_PAGED_OUT_K:
      mmem_paged_out_K = s_map[i].mv_ui32;
      break;
    case MMEM_PAGES_SCANNED:
      mmem_pages_scanned = s_map[i].mv_ui32;
      break;
    case MMEM_FREE_MEMORY_K:
      mmem_free_memory_K = s_map[i].mv_ui32;
      break;
    case MMEM_MEMORY_FREED_K:
      mmem_memory_freed_K = s_map[i].mv_ui32;
      break;
    case MMEM_PAGE_FAULTS:
      mmem_page_faults = s_map[i].mv_ui32;
      break;
    case MMEM_MAJOR_FAULTS:
      mmem_major_faults = s_map[i].mv_ui32;
      break;
    default:
      break;
    }
  }
  net_client->mc_release(s_map);
}

}
