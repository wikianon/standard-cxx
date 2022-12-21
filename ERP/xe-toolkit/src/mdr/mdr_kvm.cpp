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


#define _MDR_KVM_CPP_

#include <unistd.h>
#include <fcntl.h>
#include <nlist.h>
#include "mdr_kvm.h"

namespace mdr {

template <typename kvm_type> int mdr_kvm_t<kvm_type>::kmem_open(void)
{
  mk_kmem = (void *) kvm_open(0, 0, 0, O_RDONLY, 0);
  if (mk_kmem == 0)
    return -1;
  return 0;
}

template <typename kvm_type> int mdr_kvm_t<kvm_type>::kmem_read(void)
{
  int n;

#ifdef _SOLARIS
  n = kvm_kread((kvm_t *) mk_kmem, (uintptr_t) mk_address,
                (void *) &mk_value, sizeof(kvm_type));
#endif
#if defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
  n = kvm_read((kvm_t *) mk_kmem, (uintptr_t) mk_address,
               (void *) &mk_value, sizeof(kvm_type));
#endif
  return n;
}

template <typename kvm_type> void mdr_kvm_t<kvm_type>::kmem_close(void)
{
  kvm_close((kvm_t *) mk_kmem);
}

template <typename kvm_type> int mdr_kvm_t<kvm_type>::kmem_nlist(void)
{
#ifdef _AIX
  struct nlist64 nl[2];
#else
  struct nlist nl[2];
#endif
  int n;

  memset(&nl, '\0', sizeof nl);
  nl[0].n_name = (char *) mk_name;
  n = kvm_nlist((kvm_t *) mk_kmem, nl);
  mk_address = (caddr_t) nl[0].n_value;
  return n;
}

}
