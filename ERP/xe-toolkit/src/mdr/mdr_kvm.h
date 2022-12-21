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


#ifndef _MDR_KVM_H_
#define _MDR_KVM_H_

#ifdef _AIX
// opaque type declared in mdr_kvm.cpp
extern "C" typedef struct _kvmd kvm_t;
#else
# include <kvm.h>
#endif
#include "mdr_types.h"
#include "mdr_exception.h"

namespace mdr {

template <typename kvm_type>
class mdr_kvm_t {
private:
  // vars
  void       *mk_kmem;
  const char *mk_name;
  caddr_t     mk_address;
  kvm_type    mk_value;

  // methods
  int         kmem_open(void);      // open /dev/kmem
  int         kmem_read(void);      // read /dev/kmem
  void        kmem_close(void);     // close /dev/kmem
  int         kmem_nlist(void);     // nlist the kernel
public:
  mdr_kvm_t(const char *name)
  {
    if (kmem_open() == -1)
      throw mdr_exception_t("mdr_kvm_t::mdr_kvm_t: cannot kmem_open");
    mk_name = name;
    if ((kmem_nlist() == -1) || (mk_address == 0))
      throw mdr_exception_t("mdr_kvm_t::mdr_kvm_t: cannot find %s", mk_name);
    mk_refresh();
  }

  mdr_kvm_t(caddr_t address)
  {
    if (kmem_open() == -1)
      throw mdr_exception_t("mdr_kvm_t::mdr_kvm_t: cannot kmem_open");
    mk_address = address;
    mk_refresh();
  }

 ~mdr_kvm_t()
  {
    kmem_close();
  }

  void mk_refresh(void)
  {
    if (kmem_read() == -1)
      throw mdr_exception_t("mdr_kvm_t::refresh: cannot read_kvm");
  }

  kvm_type &operator*()
  {
    return mk_value;
  }

  kvm_type *operator->()
  {
    return &mk_value;
  }
};

#ifdef _AIX

extern "C" {
  extern void *kvm_open(const char *namelist, const char *corefile,
                         const char *swapfile, int flag, const char *errstr);
  extern void kvm_close(void *kd);
  extern int kvm_read(void *kd, ulong_t addr, void *buf, size_t nbytes);
  extern int kvm_nlist(void *kd, struct nlist64 *nl);
}

#endif

}

#if defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
# ifndef _MDR_KVM_CPP_
#  include "mdr_kvm.cpp"
# endif
#endif

#endif
