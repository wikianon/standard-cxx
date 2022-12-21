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
#include "mdr_pf.h"

using namespace mdr;

int main(int argc, char *argv[])
{
  try {
    mdr_pf_t pf(argv[1]);
    mdr_t &mdr = pf;

    // prime the pump
    mdr.mdr_rates();

    std::cout << _GT_("Category") << ": " << mdr.mdr_category_name << std::endl;
    std::cout << _GT_("Singleton") << ": " << (mdr.mdr_is_singleton ? _GT_("yes") : _GT_("no"))
              << std::endl;
    std::cout << _GT_("Supported") << ": " << (mdr.mdr_category_supported ? _GT_("yes") : _GT_("no"))
              << std::endl << std::endl;

    if (!mdr.mdr_category_supported)
      return 0;

    mdr_value_t v;

    if (mdr.mdr_is_singleton) {
      for(int i=0; i<mdr.mdr_field_count; i++)
        std::cout << mdr.mdr_field_name(i) << ": " << mdr.mdr_field_value(i, v)
                  << " (" << (mdr.mdr_is_supported(i) ? _GT_("yes") : _GT_("no"))
                  << ")" << std::endl;

      std::cout << std::endl << "ZZZzzz" << std::endl << std::endl;

      sleep(1);
      mdr.mdr_refresh_rates();

      for(int i=0; i<mdr.mdr_field_count; i++)
        std::cout << mdr.mdr_field_name(i) << ": " << mdr.mdr_field_value(i, v)
                  << " (" << (mdr.mdr_is_supported(i) ? _GT_("yes") : _GT_("no"))
                  << ")" << std::endl;
    } else {
      mdr_keyset_t keyset;
      mdr_key_t key;

      mdr.mdr_keyset(keyset);

      for(key=keyset.begin(); key != keyset.end(); key++) {
        std::cout << "-----------------" << std::endl;
        std::cout << _GT_("Key") << ": " << *key << std::endl;
        for(int i=0; i<mdr.mdr_field_count; i++)
          std::cout << mdr.mdr_field_name(i) << ": "
                    << mdr.mdr_field_value(*key, i, v)
                    << " (" << (mdr.mdr_is_supported(i) ? _GT_("yes") : _GT_("no"))
                    << ")" << std::endl;
      }

      std::cout << std::endl << "ZZZzzz" << std::endl << std::endl;

      sleep(1);
      mdr.mdr_refresh_rates();

      for(key=keyset.begin(); key != keyset.end(); key++) {
        std::cout << "-----------------" << std::endl;
        std::cout << _GT_("Key") << ": " << *key << std::endl;
        for(int i=0; i<mdr.mdr_field_count; i++)
          std::cout << mdr.mdr_field_name(i) << ": "
                    << mdr.mdr_field_value(*key, i, v)
                    << " (" << (mdr.mdr_is_supported(i) ? _GT_("yes") : _GT_("no"))
                    << ")" << std::endl;
      }
    }
  } catch(std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
