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


#include <unistd.h>
#include "mdr_ctlrstat.h"

using namespace mdr;

mdr_ctlrstat_t::mdr_ctlrstat_t(const char *remote_host) :
  mdr_ctlr_t(remote_host)
{
  mdr_category_name = MDR_CTLRSTAT_NAME;
  mdr_explanation_text = _GX_("CtlrStat_Category");
  misc = new mdr_misc_t(remote_host);
  last_time.tv_sec = misc->mm_boot_time;
  last_time.tv_usec = 0;

  mctl_recompute();
}

mdr_ctlrstat_t::~mdr_ctlrstat_t()
{
  delete misc;
}

void mdr_ctlrstat_t::mctl_recompute(void)
{
  mctl_node_t n;
  mctl_can_t *cp;
  uint_t io_ops;
  timeval now_tod;
  ulong_t elapsed; // in milliseconds

  gettimeofday(&now_tod, 0);
  elapsed = (ulong_t)
    ((last_time.tv_sec * 1000.0 + (last_time.tv_usec / 1000.0)) -
    (now_tod.tv_sec * 1000.0 + (last_time.tv_usec / 1000.0)));
  
  for(n=begin(); n != end(); n++) {
    cp = &n->second;

    // if there's no activity, then there's nothing to compute
    io_ops = cp->mctl_io_operations;
    if (io_ops == 0) {
      cp->clear();
      continue;
    }

    // service time uses the run msecs; it's the average access time.
    cp->mctl_service_time = (double) cp->mctl_runq_msecs / (double) io_ops;

    // 3 disks at 100% == 300% / 3 == 100%
    cp->mctl_util_percent =
      (((double) cp->mctl_run_msecs * 100.0) /
        (double) elapsed) / (double) cp->mctl_disk_count;

#ifdef _AIX
    // Every time an op is put in the queue, the accumulator is increased
    // by the length of the queue. I think. ala: "q_sampled += qdepth;".
    // Ergo, if you divide the delta of the sampled length by the number
    // of operations in the interval, you should get the average queue length.
    // If I had access to AIX source, I could hash this out.

    cp->mctl_avg_queue_length /= io_ops;
#else
    cp->mctl_avg_queue_length = cp->mctl_avg_queue_length / elapsed;
#endif
    cp->mctl_reads     = cp->mctl_reads;
    cp->mctl_read_K    = cp->mctl_read_K;
    cp->mctl_writes    = cp->mctl_writes;
    cp->mctl_written_K = cp->mctl_written_K;

    // if there are no ops, then there is no activity
    if (cp->mctl_io_operations == 0) {
      cp->mctl_avg_queue_length = 0;
      cp->mctl_service_time = 0;
      cp->mctl_util_percent = 0;
    }

    // there are those cases...
    if (cp->mctl_util_percent > 100.0)
      cp->mctl_util_percent = 100.0;
  }
}

void mdr_ctlrstat_t::mdr_refresh(void)
{
  misc->mdr_refresh_rates();
  mdr_ctlr_t::mdr_refresh();
  mdr_ctlr_t::mdr_rates();
  mctl_recompute();
}
