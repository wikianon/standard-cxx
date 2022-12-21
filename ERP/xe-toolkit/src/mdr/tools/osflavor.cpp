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


#include <unistd.h>
#include <mdr_perf_host.h>

using namespace mdr;

int main(int argc, char *argv[])
{
  try {
    const char *remote_host = 0;
    int c;

    opterr = 0;
    while((c = getopt(argc, argv, "h:")) != -1)
      switch(c) {
      case 'h':
        remote_host = optarg;
        break;
      default:
        std::cerr << "Use: " << argv[0] << " [-h remote_host]"
                  << std::endl;
        return 1;
      }

    mdr_performance_host_t host(remote_host);

    std::cout << host.mdr_os_flavor << std::endl;
  } catch(std::exception &e) {
    std::cout << argv[0] << ": " << e.what() << std::endl;
  }

  return 0;
}
