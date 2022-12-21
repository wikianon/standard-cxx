#ifdef _AIX

#include <fcntl.h>
#include <nlist.h>
#include "mdr_types.h"

extern "C" {

// kvm_t type from mdr_kvm.h
typedef struct _kvmd {
  int fd;
} kvm_t;

void *kvm_open(const char *namelist, const char *corefile,
               const char *swapfile, int flag, const char *errstr)
{
  kvm_t *kd = new kvm_t;

  kd->fd = open64("/dev/kmem", flag);
  if (kd->fd == -1) {
    delete kd;
    return 0;
  }
  return kd;
}

void kvm_close(void *vkd)
{
  kvm_t *kd = (kvm_t *) vkd;

  if (kd == 0)
    return;
  if (kd->fd > 0)
    close(kd->fd);
  delete kd;
}

int kvm_read(void *vkd, ulong_t addr, void *buf, size_t nbytes)
{
  kvm_t *kd = (kvm_t *) vkd;

  if (lseek64(kd->fd, addr, SEEK_SET) == -1)
    return -1;
  if (read(kd->fd, buf, nbytes) != nbytes)
    return -1;
  return nbytes;
}

int kvm_nlist(void *kd, struct nlist64 *nl)
{
  return knlist((struct nlist *) nl, 1, sizeof(struct nlist64));
}

}
#endif
