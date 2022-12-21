/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr tools of Captivity.
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


#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#ifdef _SOLARIS
# include <sys/termios.h>
#endif
#include <mdr_ctlrstat.h>

using namespace mdr;

static inline void header(void)
{
  puts("device             %util   qlen (ios:kB rd  ios:kB wr"
       "  ios:kB /sec) avwait (ms)");
}

int main(int argc, char *argv[])
{
  try {
    int c;
    int interval = 1;
    int iterations = 1;
    char *wanted_ctlr = 0;
    int chatty_header = 0;
    int line_count = 20;
    int lines_til_header = line_count;
    struct winsize winsz;
    bool do_support_matrix = false;
    const char *remote_host = 0;
    mctl_node_t n;

    opterr = 0;
    while((c = getopt(argc, argv, "cn:sh:")) != -1) {
      switch(c) {
      case 'c':
        chatty_header = 1;
        break;
      case 'n':
        wanted_ctlr = optarg;
        break;
      case 's':
        do_support_matrix = true;
        break;
      case 'h':
        remote_host = optarg;
        break;
      default:
        std::cerr << "Use: " << argv[0]
                  << "[-cs] [-n ctlr] [-h remote_host] [interval [iterations]]"
                  << std::endl;
        return 1;
      }
    }

    mdr_ctlrstat_t ctlr(remote_host);

    if (do_support_matrix) {
      mdr_t::mdr_support_matrix(ctlr, std::cout);
      return 0;
    }

    if (argv[optind]) {
      interval = atoi(argv[optind]);
      if (argv[optind+1])
        iterations = atoi(argv[optind+1]);
      else
        iterations = 0;
    }

    if (chatty_header == 0) {
      if (ioctl(1, TIOCGWINSZ, &winsz) != -1)
        line_count = winsz.ws_row - 3;
      lines_til_header = line_count;
      header();
    }

    do {
      if (chatty_header)
        header();
      for(n=ctlr.begin(); n != ctlr.end(); n++) {
        const std::string &name = n->first;
        const mctl_can_t &can = n->second;

        if ((wanted_ctlr == 0) || (name == wanted_ctlr))
          printf("%-16.16s %6.2f%% %6.2f  %3u:%-6u %3u:%-6u "
                 "%3u:%-6u %8.1f\n",
                   name.c_str(),
                   can.mctl_util_percent, can.mctl_avg_queue_length,
                   can.mctl_reads, can.mctl_read_K,
                   can.mctl_writes, can.mctl_written_K,
                   can.mctl_io_operations,
                   can.mctl_read_K + can.mctl_written_K,
                   can.mctl_service_time);
      }

      if (iterations != 1) {
        sleep(interval);
        ctlr.mdr_refresh();
        if ((chatty_header == 0) && (--lines_til_header == 0)) {
          lines_til_header = line_count;
          header();
        }
      }
    } while(--iterations);
  } catch(std::exception &e) {
    std::cout << argv[0] << ": " << e.what() << std::endl;
  }

  return 0;
}
