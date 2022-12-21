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


#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <mdr_mnt.h>

using namespace mdr;

int main(int argc, char *argv[])
{
  try {
    mmnt_node_t node;
    mmnt_can_t *cp;
    char capacity[128];
    int c;
    const char *remote_host = 0;
    bool do_support_matrix = false;

    opterr = 0;
    while((c = getopt(argc, argv, "sh:")) != -1)
      switch(c) {
      case 's':
        do_support_matrix = true;
        break;
      case 'h':
        remote_host = optarg;
        break;
      default:
        std::cerr << "Use: " << argv[0] << " [-s]" << std::endl;
        return 1;
      }

    mdr_mnt_t mnt(remote_host);

    if (do_support_matrix) {
      mdr_t::mdr_support_matrix(mnt, std::cout);
      return 0;
    }

    puts("Filesystem            kbytes    used      avail     util Mounted on");
    for(node=mnt.begin(); node != mnt.end(); node++) {
      cp = &node->second;
      if (strstr(cp->mmnt_mount_options.c_str(), "ignore"))
        continue;
      if (cp->mmnt_size_K == 0)
        strcpy(capacity, "-");
      else 
        snprintf(capacity, sizeof capacity, "%.0f%%", cp->mmnt_percent_used);
      const char *special = cp->mmnt_special.c_str();
      if (strlen(special) > 21) {
        puts(special);
        special = "";
      }
      printf("%-21.21s %-9ld %-9ld %-9ld %-4.4s %s\n",
        special, cp->mmnt_size_K, cp->mmnt_used_K, cp->mmnt_free_K,
        capacity, cp->mmnt_mount_point.c_str());
    }
  } catch(std::exception &e) {
    std::cout << argv[0] << ": " << e.what() << std::endl;
  }
  return 0;
}
