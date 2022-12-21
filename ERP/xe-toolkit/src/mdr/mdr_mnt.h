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


#ifndef _MDR_MNT_H_
#define _MDR_MNT_H_

#include "mdr.h"

namespace mdr {

const int MMNT_VERSION = 1;

const int MMNT_SPECIAL        = 0;
const int MMNT_MOUNT_POINT    = 1;
const int MMNT_FS_TYPE        = 2;
const int MMNT_MOUNT_OPTIONS  = 3;
const int MMNT_MOUNT_TIME     = 4;
const int MMNT_SIZE           = 5;
const int MMNT_USED_K         = 6;
const int MMNT_FREE_K         = 7;
const int MMNT_PERCENT_USED   = 8;
const int MMNT_FILES          = 9;
const int MMNT_FREE_FILES     = 10;
const int MMNT_MAJOR          = 11;
const int MMNT_MINOR          = 12;
const int MMNT_FILE_SYSTEM_ID = 13;
const int MMNT_FIELD_COUNT    = 14;

class mmnt_can_t {
public:
  std::string mmnt_special;         // Special File
  std::string mmnt_mount_point;     // Mount Point
  std::string mmnt_fs_type;         // File System Type
  std::string mmnt_mount_options;   // Mount Options
  long        mmnt_mount_time;      // Mount Time
  ulong_t     mmnt_size_K;          // KBytes Space Total
  ulong_t     mmnt_used_K;          // KBytes Space Used
  ulong_t     mmnt_free_K;          // KBytes Space Free
  double      mmnt_percent_used;    // Percent Used
  ulong_t     mmnt_files;           // Inode Count
  ulong_t     mmnt_free_files;      // Free Inode Count
  int         mmnt_major;           // Major Number
  int         mmnt_minor;           // Minor Number
  uint64_t    mmnt_file_system_id;  // File System ID

  mmnt_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mmnt_special.clear();
    mmnt_mount_point.clear();
    mmnt_fs_type.clear();
    mmnt_mount_options.clear();
    mmnt_mount_time = 0;
    mmnt_size_K = 0;
    mmnt_used_K = 0;
    mmnt_free_K = 0;
    mmnt_percent_used = 0;
    mmnt_files = 0;
    mmnt_free_files = 0;
    mmnt_major = 0;
    mmnt_minor = 0;
    mmnt_file_system_id = 0;
  }
};

typedef std::map<std::string, mmnt_can_t> mmnt_box_t;
typedef mmnt_box_t::iterator mmnt_node_t;

class mdr_mnt_t : public mdr_t, public mmnt_box_t {
public:
  // constructors/destructors
  mdr_mnt_t(const char *remote_host = 0);
 ~mdr_mnt_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(field_number, value);
  }
  const mdr_keyset_t &mdr_keyset(mdr_keyset_t &keyset);

private:
  mmnt_box_t   old;

  void mnt_remote_refresh(void);
};

} // end of namespace mdr

#endif
