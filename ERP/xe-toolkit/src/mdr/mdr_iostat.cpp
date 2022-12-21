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
#include "mdr_iostat.h"

using namespace mdr;

mdr_iostat_t::mdr_iostat_t(const char *remote_host) : mdr_disk_t(remote_host)
{
  mdr_category_name = MDR_IOSTAT_NAME;
  mdr_explanation_text = _GX_("IoStat_Category");
  misc = new mdr_misc_t(remote_host);
  last_time.tv_sec = misc->mm_boot_time;
  last_time.tv_usec = 0;

  // what these fields do must be reflected in their names
  mdr_field_names[MD_READS]         = "Read Operations/sec";
  mdr_field_names[MD_READ_K]        = "KBytes Read/sec";
  mdr_field_names[MD_WRITES]        = "Write Operations/sec";
  mdr_field_names[MD_WRITTEN_K]     = "KBytes Written/sec";
  mdr_field_names[MD_IO_OPERATIONS] = "Total Transfers/sec";

  // map the new names
  mdr_map_names();

  mio_recompute();
}

mdr_iostat_t::~mdr_iostat_t()
{
  delete misc;
}

void mdr_iostat_t::mio_recompute(void)
{
  md_node_t n;
  md_can_t *cp;
  uint_t io_ops;
  timeval now_tod;
  double elapsed; // in milliseconds

  gettimeofday(&now_tod, 0);
  elapsed =
    ((double) now_tod.tv_sec * 1000.0 + (now_tod.tv_usec / 1000.0)) -
    (((double) last_time.tv_sec * 1000.0 + (last_time.tv_usec / 1000.0)));
  last_time = now_tod;

  // divide-by-zero is unpleasant
  if (elapsed == 0.0)
    elapsed = 1.0;
  
  for(n=begin(); n != end(); n++) {
    cp = &n->second;

    // if there's no activity, then there's nothing to compute
    io_ops = cp->md_io_operations;
    if (io_ops == 0) {
      uint_t space = cp->md_space_mb;
      cp->clear();
      cp->md_space_mb = space;
      continue;
    }

    // service time uses the run msecs; it's the average access time.
    cp->md_service_time = (double) cp->md_runq_msecs / (double) io_ops;

    cp->md_util_percent =
      ((double) cp->md_run_msecs * 100.0) / elapsed;

#ifdef _AIX
    // Every time an op is put in the queue, the accumulator is increased
    // by the length of the queue. I think. ala: "q_sampled += qdepth;".
    // Ergo, if you divide the delta of the sampled length by the number
    // of operations in the interval, you should get the average queue length.
    // If I had access to AIX source, I could hash this out.

    cp->md_avg_queue_length /= io_ops;
#else
    cp->md_avg_queue_length = cp->md_avg_queue_length / elapsed;
#endif
    cp->md_reads         = (uint_t) (cp->md_reads / (elapsed / 1000.0));
    cp->md_read_K        = (uint_t) (cp->md_read_K / (elapsed / 1000.0));
    cp->md_writes        = (uint_t) (cp->md_writes / (elapsed / 1000.0));
    cp->md_written_K     = (uint_t) (cp->md_written_K / (elapsed / 1000.0));
    cp->md_io_operations = (uint_t) (cp->md_io_operations / (elapsed / 1000.0));

    // if there are no ops, then there is no activity
    if (cp->md_io_operations == 0) {
      cp->md_avg_queue_length = 0;
      cp->md_service_time = 0;
      cp->md_util_percent = 0;
    }

    // there are those cases...
    if (cp->md_util_percent > 100.0)
      cp->md_util_percent = 100.0;
  }
}

void mdr_iostat_t::mdr_refresh(void)
{
  mdr_disk_t::mdr_refresh();
  mdr_disk_t::mdr_rates();
  mio_recompute();
}
