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
#include <mdr_iostat.h>
#include <mdr_mnt.h>
#include <mdr_exception.h>

using namespace mdr;

static inline void
header (void)
{
  puts ("device           %util   qlen (rio:kB rd  wio:kB wr"
        "  ios:kB /sec) svctim (ms)");
}

int
main (int argc, char *argv[])
{
  try
  {
    md_node_t n;
    md_can_t *cp;
    std::string name;
    char *wanted_disk = 0;
    int c;
    bool printit;
    int interval = 1;
    int iterations = 1;
    bool show_disks = false;
    bool chatty_header = false;
    bool net_disk = false;
    int line_count = 20;
    int lines_til_header = line_count;
    struct winsize winsz;
    const char *remote_host = 0;
    bool do_support_matrix = false;

    opterr = 0;
    while ((c = getopt (argc, argv, "cdNn:sh:")) != -1)
      {
        switch (c)
          {
          case 'c':
            chatty_header = true;
            break;
          case 'd':
            show_disks = true;
            break;
          case 'n':
            wanted_disk = optarg;
            break;
          case 'N':
            net_disk = true;
            break;
          case 's':
            do_support_matrix = true;
            break;
          case 'h':
            remote_host = optarg;
            break;
          default:
            fprintf(stderr, "Use: %s [-cdNs] [-n disk] [-h remote_host] "
                            "[interval [iterations]]\n",
                     argv[0]);
            return 1;
          }
      }

    mdr_iostat_t io(remote_host);
    mdr_mnt_t mnt(remote_host);

    if (do_support_matrix) {
      mdr_t::mdr_support_matrix(io, std::cout);
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

    if (!chatty_header)
      {
        if (ioctl (1, TIOCGWINSZ, &winsz) != -1)
          line_count = winsz.ws_row - 3;
        lines_til_header = line_count;
        header ();
      }

    do
      {
        if (chatty_header)
          header ();
        for (n = io.begin (); n != io.end (); n++)
          {
            cp = &n->second;
            name = n->first;
            printit = false;
            mmnt_node_t mn = mnt.find(name);
            if (mn != mnt.end())
              name = mn->second.mmnt_mount_point;

            // if wanted_disk and this is it
            if (wanted_disk)
              {
                if (name == wanted_disk)
                  printit = true;
              }
            else
              {
                if (cp->md_type == "logical")
                  {
                    if (!show_disks)
                      printit = true;
                  }
                else if (cp->md_type == "physical")
                  {
                    if (show_disks)
                      printit = true;
                  }
                else if (cp->md_type == "network")
                  {
                    if (net_disk)
                      printit = true;
                  }
              }

            if (printit) {
              printf
                ("%-14.14s %6.2f%% %6.2f  %3u:%-6u %3u:%-6u %3u:%-6u %8.1f\n",
                 name.c_str(), cp->md_util_percent,
                 cp->md_avg_queue_length, cp->md_reads, cp->md_read_K,
                 cp->md_writes, cp->md_written_K,
                 cp->md_io_operations,
                 cp->md_read_K + cp->md_written_K,
                 cp->md_service_time);

              if (!chatty_header && (--lines_til_header == 0))
                {
                  lines_til_header = line_count;
                  header ();
                }
            }
          }
        if (iterations != 1)
          {
            sleep (interval);
            io.mdr_refresh();
          }
      }
    while (--iterations);
  }
  catch (std::exception & e)
  {
    std::cout << argv[0] << ": " << e.what () << std::endl;
  }

  return 0;
}
