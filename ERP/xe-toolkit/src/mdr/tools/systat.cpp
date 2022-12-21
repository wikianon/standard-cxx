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
#include <unistd.h>
#include <sys/ioctl.h>
#ifdef _SOLARIS
# include <sys/termios.h>
#endif
#include <mdr_systat.h>

using namespace mdr;

static inline void header(void)
{
  puts("-procs-- -mem- --pf- ----vm--- ----io--- "
       "-intr- -----flt----- ------cpu%-----");
  puts(" r  b  z %free %free  Kpi  Kpo   Kr   Kw  "
       "total    vcs    sys   u   k   w   i");

}

int main (int argc, char *argv[])
{
  try
  {
    int c;
    int interval = 1;
    int iterations = 1;
    int line_count = 20;
    int lines_til_header;
    struct winsize winsz;
    bool do_support_matrix = false;
    const char *remote_host = 0;

    opterr = 0;
    while ((c = getopt (argc, argv, "sh:")) != -1)
      {
        switch (c)
          {
          case 's':
            do_support_matrix = true;
            break;
          case 'h':
            remote_host = optarg;
            break;
          default:
            fprintf (stderr, "Use: %s [-s] [interval [iterations]]\n",
                     argv[0]);
            return 1;
          }
      }

    mdr_systat_t sys(remote_host);

    if (do_support_matrix) {
      mdr_t::mdr_support_matrix(sys, std::cout);
      return 0;
    }

    if (argv[optind])
      {
        interval = atoi (argv[optind]);
        if (argv[optind + 1])
          iterations = atoi (argv[optind + 1]);
        else
          iterations = 0;
      }

    // if I can't get the winsz, punt
    if (ioctl (1, TIOCGWINSZ, &winsz) != -1)
      line_count = winsz.ws_row - 3;
    lines_til_header = line_count;
    header();

    do
      {
        printf("%2u %2u %2u %5.1f %5.1f %4u %4u "
               "%4u %4u %6u %6u %6u %3u %3u %3u %3u\n",
          sys.msys_procs_runnable,
          sys.msys_procs_blocked,
          sys.msys_procs_zombied,
          sys.msys_free_memory_percent,
          sys.msys_free_paging_file_percent,
          sys.msys_page_K_in_per_second,
          sys.msys_page_K_out_per_second,
          sys.msys_read_K_per_second,
          sys.msys_written_K_per_second,
          sys.msys_interrupts_per_second,
          sys.msys_context_switches_per_second,
          sys.msys_system_calls_per_second,
          sys.msys_user_time_percent,
          sys.msys_kernel_time_percent,
          sys.msys_wait_time_percent,
          sys.msys_idle_time_percent);

        if (iterations != 1)
          {
            sleep (interval);
            sys.mdr_refresh();
            if (--lines_til_header == 0)
              {
                lines_til_header = line_count;
                header ();
              }
          }
      }
    while (--iterations);
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
