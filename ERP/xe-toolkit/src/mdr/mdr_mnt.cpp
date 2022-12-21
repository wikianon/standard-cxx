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


#include "mdr_mnt.h"

namespace mdr {

mdr_mnt_t::mdr_mnt_t(const char *remote_host) :
  mdr_t(MDR_MNT_NAME, true, MMNT_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_mnt_t", remote_host, MMNT_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MMNT_SPECIAL);
    mdr_support_field(MMNT_MOUNT_POINT);
    mdr_support_field(MMNT_FS_TYPE);
    mdr_support_field(MMNT_MOUNT_OPTIONS);
#if defined(_SOLARIS) || defined(_AIX)
    mdr_support_field(MMNT_MOUNT_TIME);
#endif
    mdr_support_field(MMNT_SIZE);
    mdr_support_field(MMNT_USED_K);
    mdr_support_field(MMNT_FREE_K);
    mdr_support_field(MMNT_PERCENT_USED);
    mdr_support_field(MMNT_FILES);
    mdr_support_field(MMNT_FREE_FILES);
    mdr_support_field(MMNT_MAJOR);
    mdr_support_field(MMNT_MINOR);
#if defined(_SOLARIS) || defined(_AIX)
    mdr_support_field(MMNT_FILE_SYSTEM_ID);
#endif

    net_client = 0;
  }

  mdr_field_names[MMNT_SPECIAL]        = "Special File";
  mdr_field_names[MMNT_MOUNT_POINT]    = "Mount Point";
  mdr_field_names[MMNT_FS_TYPE]        = "File System Type";
  mdr_field_names[MMNT_MOUNT_OPTIONS]  = "Mount Options";
  mdr_field_names[MMNT_MOUNT_TIME]     = "Mount Time";
  mdr_field_names[MMNT_SIZE]           = "KBytes Space Total";
  mdr_field_names[MMNT_USED_K]         = "KBytes Space Used";
  mdr_field_names[MMNT_FREE_K]         = "KBytes Space Free";
  mdr_field_names[MMNT_PERCENT_USED]   = "Percent Used";
  mdr_field_names[MMNT_FILES]          = "Inode Count";
  mdr_field_names[MMNT_FREE_FILES]     = "Free Inode Count";
  mdr_field_names[MMNT_MAJOR]          = "Major Number";
  mdr_field_names[MMNT_MINOR]          = "Minor Number";
  mdr_field_names[MMNT_FILE_SYSTEM_ID] = "File System ID";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Mounts_Category");

  mdr_refresh_rates();
}

mdr_mnt_t::~mdr_mnt_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_mnt_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mmnt_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_mnt_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MMNT_SPECIAL:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mmnt_special;
    break;
  case MMNT_MOUNT_POINT:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mmnt_mount_point;
    break;
  case MMNT_FS_TYPE:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mmnt_fs_type;
    break;
  case MMNT_MOUNT_OPTIONS:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mmnt_mount_options;
    break;
  case MMNT_MOUNT_TIME:
    value.mv_type = MVT_LONG;
    value.mv_l = n->second.mmnt_mount_time;
    break;
  case MMNT_SIZE:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mmnt_size_K;
    break;
  case MMNT_USED_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mmnt_used_K;
    break;
  case MMNT_FREE_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mmnt_free_K;
    break;
  case MMNT_PERCENT_USED:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mmnt_percent_used;
    break;
  case MMNT_FILES:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mmnt_files;
    break;
  case MMNT_FREE_FILES:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mmnt_free_files;
    break;
  case MMNT_MAJOR:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mmnt_major;
    break;
  case MMNT_MINOR:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mmnt_minor;
    break;
  case MMNT_FILE_SYSTEM_ID:
    value.mv_type = MVT_UI64;
    value.mv_ui64 = n->second.mmnt_file_system_id;
    break;
  }
  return value;
}

void mdr_mnt_t::mdr_rates(void)
{
  mmnt_node_t n;
  mmnt_node_t nn;
  mmnt_can_t *cp;
  mmnt_can_t *ocp;
  ulong_t tmp_free_K;
  ulong_t tmp_free_files;

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_free_K = cp->mmnt_free_K;
    tmp_free_files = cp->mmnt_free_files;

#ifdef _LP64
    cp->mmnt_free_K -= ocp->mmnt_free_K;
    cp->mmnt_free_files -= ocp->mmnt_free_files;
#else
    cp->mmnt_free_K =
      wrap_diff_32(cp->mmnt_free_K, ocp->mmnt_free_K);
    cp->mmnt_free_files =
      wrap_diff_32(cp->mmnt_free_files, ocp->mmnt_free_files);
#endif

    ocp->mmnt_free_K = tmp_free_K;
    ocp->mmnt_free_files = tmp_free_files;
  }
}

const mdr_keyset_t &mdr_mnt_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mmnt_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_mnt_t::mnt_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mmnt_can_t can;
  int i;
  int n;

  // start fresh
  clear();

  net_client->mc_refresh(m_map);
  for(mn=m_map.begin(); mn != m_map.end(); mn++) {
    mdr_s_mm_t &s_map = mn->second;

    can.clear();

    for(i=0, n=s_map.size(); i<n; i++) {
      switch(i) {
      case MMNT_SPECIAL:
        can.mmnt_special = *s_map[i].mv_s;
        break;
      case MMNT_MOUNT_POINT:
        can.mmnt_mount_point = *s_map[i].mv_s;
        break;
      case MMNT_FS_TYPE:
        can.mmnt_fs_type = *s_map[i].mv_s;
        break;
      case MMNT_MOUNT_OPTIONS:
        can.mmnt_mount_options = *s_map[i].mv_s;
        break;
      case MMNT_MOUNT_TIME:
        switch(s_map[i].mv_type) {
        case MVT_I32:
          can.mmnt_mount_time = s_map[i].mv_i32;
          break;
        case MVT_I64:
          can.mmnt_mount_time = s_map[i].mv_i64;
          break;
        default:
          can.mmnt_mount_time = -1;
          break;
        }
        break;
      case MMNT_SIZE:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mmnt_size_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mmnt_size_K = s_map[i].mv_ui64;
          break;
        default:
          can.mmnt_size_K = (ulong_t) -1;
          break;
        }
        break;
      case MMNT_USED_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mmnt_used_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mmnt_used_K = s_map[i].mv_ui64;
          break;
        default:
          can.mmnt_used_K = (ulong_t) -1;
          break;
        }
        break;
      case MMNT_FREE_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mmnt_free_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mmnt_free_K = s_map[i].mv_ui64;
          break;
        default:
          can.mmnt_free_K = (ulong_t) -1;
          break;
        }
        break;
      case MMNT_PERCENT_USED:
        can.mmnt_percent_used = s_map[i].mv_d;
        break;
      case MMNT_FILES:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mmnt_files = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mmnt_files = s_map[i].mv_ui64;
          break;
        default:
          can.mmnt_files = (ulong_t) -1;
          break;
        }
        break;
      case MMNT_FREE_FILES:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mmnt_free_files = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mmnt_free_files = s_map[i].mv_ui64;
          break;
        default:
          can.mmnt_free_files = (ulong_t) -1;
          break;
        }
        break;
      case MMNT_MAJOR:
        can.mmnt_major = s_map[i].mv_i32;
        break;
      case MMNT_MINOR:
        can.mmnt_minor = s_map[i].mv_i32;
        break;
      case MMNT_FILE_SYSTEM_ID:
        can.mmnt_file_system_id = s_map[i].mv_ui64;
        break;
      default:
        break;
      }
    }

    (*this)[mn->first] = can;
  }

  net_client->mc_release(m_map);
}

}
