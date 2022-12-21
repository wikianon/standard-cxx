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


#ifndef _MDR_IOSTAT_H_
#define _MDR_IOSTAT_H_

#include <mdr_misc.h>
#include <mdr_disk.h>

namespace mdr {

class mdr_iostat_t : public mdr_disk_t {
public:
  mdr_iostat_t(const char *remote_host = 0);
  ~mdr_iostat_t();

  void mdr_refresh(void);
  void mdr_refresh_rates(void)
  {
    mdr_iostat_t::mdr_refresh();
    // no rates
  }
  void mdr_rates(void)
  {
    // no rates
  }

private:
  long        hertz;
  mdr_misc_t *misc;
  timeval     last_time;

  void mio_recompute(void);
};

}

#endif
