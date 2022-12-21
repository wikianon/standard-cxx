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


#define NOMACROS
#include <iostream>
#include <unistd.h>
#include <curses.h>
#include <mdr_ps.h>
#include <mdr_thread.h>

using namespace mdr;

int main(int argc, char *argv[])
{
  try {
    char buf[BUFSIZ];
    mps_can_t pc;
    double total;
    const char *cmd;
    int c;
    int mins;
    int secs;
    int line;
    bool minus_c = false;
    bool minus_p = false;
    bool minus_t = false;
    bool show_threads = false;
    bool show_sysprocs = false;
    static char state_chars[] = {
      'R', 'S', 'T', 'W', 'Z', 'P'
    };
    int interval = 0;
    int iterations = 1;
    bool do_support_matrix = false;
    const char *remote_host = 0;
    std::string tty;

    while((c = getopt(argc, argv, "cptsh:ST")) != -1)
      switch(c) {
      case 'c':
        minus_c = true;
        break;
      case 'p':
        minus_p = true;
        break;
      case 't':
        minus_t = true;
        break;
      case 's':
        do_support_matrix = true;
        break;
      case 'h':
        remote_host = optarg;
        break;
      case 'S':
        show_sysprocs = true;
        break;
      case 'T':
        show_threads = true;
        break;
      default:
        std::cerr << "Use: " << argv[0]
                  << " [-cptT] [interval [iterations]]" << std::endl
                  << " -c - show command name (not command line)" << std::endl
                  << " -p - show cpu% & mem% (not rss & size)" << std::endl
                  << " -t - text output (not curses)" << std::endl
                  << " -S - show system processes" << std::endl
                  << " -T - show threads (not implemented)" << std::endl;
        return 1;
      }

    mdr_ps_t proc(remote_host);

    if (do_support_matrix) {
      mdr_t::mdr_support_matrix(proc, std::cout);
      return 0;
    }

    if (argv[optind]) {
      interval = atoi(argv[optind++]);
      if (interval == 0) {
        std::cerr << argv[0] << ": interval may not be zero" << std::endl;
        return 1;
      }
    }
    if (argv[optind]) {
      iterations = atoi(argv[optind++]);
      if (iterations == 0) {
        std::cerr << argv[0] << ": iterations may not be zero" << std::endl;
        return 1;
      }
    }

    if (interval != 0 && iterations == 1)
      iterations = 0x7fffffff;

    if (!minus_t)
      initscr();

    while(iterations--) {
      if (!minus_t)
        clear();
      proc.mps_sort(mdr_ps_t::BY_CPU_USE, true);
      if (minus_t) {
        if (minus_p)
          puts("   PID TTY   STAT   CPU%   MEM%     TIME COMMAND");
        else
          puts("   PID TTY   STAT   SIZE    RSS     TIME COMMAND");
      } else {
        if (minus_p)
          mvprintw(0, 0, "   PID TTY   STAT   CPU%%   MEM%%     TIME COMMAND");
        else
          mvprintw(0, 0, "   PID TTY   STAT   SIZE    RSS     TIME COMMAND");
      }
      for(line=1; proc.mps_next_sorted_proc(pc);) {
        if (!show_sysprocs && pc.mps_is_system_process)
          continue;
        total = pc.mps_cpu_time;
        mins = (int) total / 60;
        total -= (double) (mins * 60);
        secs = (int) total;
        if (pc.mps_command.length() == 0) {
          snprintf(buf, sizeof buf, "[%s]", pc.mps_file_name.c_str());
          cmd = (const char *) buf;
        } else
          if (minus_c)
            cmd = pc.mps_file_name.c_str();
          else
            cmd = pc.mps_command.c_str();

        if (minus_t) {
          if (minus_p)
            printf("%6d %-8.8s %c %6.2f %6.2f %5d:%02d %-38.38s\n",
              pc.mps_pid, proc.mps_dev_to_tty(pc.mps_tty_device, tty).c_str(),
              state_chars[pc.mps_state],
              pc.mps_cpu_percent, pc.mps_memory_percent,
              mins, secs, cmd);
          else
            printf("%6d %-8.8s %c %6ld %6ld %5d:%02d %-38.38s\n",
              pc.mps_pid, proc.mps_dev_to_tty(pc.mps_tty_device, tty).c_str(),
              state_chars[pc.mps_state],
              pc.mps_size_K, pc.mps_rssize_K,
              mins, secs, cmd);
        } else {
          if (minus_p)
            mvprintw(line, 0, "%6d %-8.8s %c %6.2f %6.2f %5d:%02d %-38.38s\n",
              pc.mps_pid, proc.mps_dev_to_tty(pc.mps_tty_device, tty).c_str(),
              state_chars[pc.mps_state],
              pc.mps_cpu_percent, pc.mps_memory_percent,
              mins, secs, cmd);
          else
            mvprintw(line, 0, "%6d %-8.8s %c %6ld %6ld %5d:%02d %-38.38s\n",
              pc.mps_pid, proc.mps_dev_to_tty(pc.mps_tty_device, tty).c_str(),
              state_chars[pc.mps_state],
              pc.mps_size_K, pc.mps_rssize_K,
              mins, secs, cmd);
          if (++line == LINES - 1)
            break;
        }

#if 0
        if (show_threads && (pc.mps_thread_count > 1)) {
          mdr_thread_t threads(pc.mps_pid, remote_host);
          mt_can_t tc;

          threads.mt_sort(mdr_thread_t::BY_CPU_USE, true);
          while(threads.mt_next_sorted_thread(tc)) {

            // skip the main thread
            if (tc.mt_thread_id == pc.mps_pid)
              continue;

            total = tc.mt_cpu_time;
            mins = (int) total / 60;
            total -= (double) (mins * 60);
            secs = (int) total;

            char parent[16];
            snprintf(parent, sizeof parent, "[%d]", pc.mps_pid);

            if (minus_t) {
              if (minus_p)
                printf("%6d %-8.8s %c %6.2f %6.2f %5d:%02d %-38.38s\n",
                  tc.mt_thread_id,
                  parent,
                  state_chars[tc.mt_state],
                  tc.mt_cpu_percent, tc.mt_memory_percent,
                  mins, secs, "[thread]");
              else
                printf("%6d %-8.8s %c %6ld %6ld %5d:%02d %-38.38s\n",
                  tc.mt_thread_id,
                  parent,
                  state_chars[tc.mt_state],
                  tc.mt_size_K, tc.mt_rssize_K,
                  mins, secs, "[thread]");
            } else {
              if (minus_p)
                mvprintw(line, 0,
                  "%6d %-8.8s %c %6.2f %6.2f %5d:%02d %-38.38s\n",
                    tc.mt_thread_id,
                    parent,
                    state_chars[tc.mt_state],
                    tc.mt_cpu_percent, tc.mt_memory_percent,
                    mins, secs, "[thread]");
              else
                mvprintw(line, 0,
                  "%6d %-8.8s %c %6ld %6ld %5d:%02d %-38.38s\n",
                    tc.mt_thread_id,
                    parent,
                    state_chars[tc.mt_state],
                    tc.mt_size_K, tc.mt_rssize_K,
                    mins, secs, "[thread]");
            }
          }
        }
#endif
      }

      if (!minus_t)
        refresh();

      if (iterations) {
        sleep(interval);
        proc.mdr_refresh_rates();
      }
    }

    if (!minus_t)
      endwin();
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
