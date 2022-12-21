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
#include <mdr_netstat.h>

#define HEAD1 \
"        ---packets--      ------bytes-----   -errors  failures --collisions--"

#define HEAD2 "\n"
#define HEAD3 \
"name    input output      input     output   in  out   in  out  count    rate"

using namespace mdr;

static inline void
header (void)
{
  puts (HEAD1 HEAD2 HEAD3);
}

int main (int argc, char *argv[])
{
  try
  {
    mns_node_t n;
    mns_can_t *cp;
    int c;
    int interval = 1;
    int iterations = 1;
    char *wanted_if = 0;
    bool printit;
    bool show_loopback = false;
    bool chatty_header = false;
    int line_count = 20;
    int lines_til_header = line_count;
    struct winsize winsz;
    const char *remote_host = 0;
    bool do_support_matrix = false;

    opterr = 0;
    while ((c = getopt (argc, argv, "cln:sh:")) != -1)
      {
        switch (c)
          {
          case 'c':
            chatty_header = true;
            break;
          case 'l':
            show_loopback = true;
            break;
          case 'n':
            wanted_if = optarg;
            break;
          case 's':
            do_support_matrix = true;
            break;
          case 'h':
            remote_host = optarg;
            break;
          default:
            fprintf (stderr,
                     "Use: %s [-cl] [-n if_name] [-h remote_host] "
                       "[interval [iterations]]\n", argv[0]);
            return 1;
          }
      }

    mdr_netstat_t net(remote_host);

    if (do_support_matrix) {
      mdr_t::mdr_support_matrix(net, std::cout);
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

    net.mdr_rates();  // prime the pump

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

        for(n=net.begin(); n != net.end(); n++)
          {
            cp = &n->second;
            printit = false;
            if (wanted_if && (n->first == wanted_if))
              printit = true;
            else if (show_loopback && (n->first.substr(0, 2) == "lo"))
              printit = true;
            else if (!wanted_if && (n->first.substr(0, 2) != "lo"))
              printit = true;
            if (printit)
              printf
               ("%-6s %6.0f %6.0f %10.0f %10.0f %4.2f %4.2f %4.2f %4.2f %6.0f %6.2f%%\n",
                 n->first.c_str(),
                 cp->mns_input_packets, cp->mns_output_packets,
                 cp->mns_input_bytes, cp->mns_output_bytes,
                 cp->mns_input_errors, cp->mns_output_errors,
                 cp->mns_input_failures, cp->mns_output_failures,
                 cp->mns_collisions, cp->mns_collision_percent);
          }
        if (iterations != 1)
          {
            sleep (interval);
            net.mdr_refresh_rates();
            if (!chatty_header && (--lines_til_header == 0))
              {
                lines_til_header = line_count;
                header();
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
