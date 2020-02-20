// See LICENSE for license details.

#include "pk.h"
#include "atomic.h"
#include "frontend.h"
#include "syscall.h"
#include "htif.h"
#include <stdint.h>
#include "hpm.h"

long frontend_syscall(long n, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6)
{
  static volatile uint64_t magic_mem[8];

  static spinlock_t lock = SPINLOCK_INIT;
  spinlock_lock(&lock);

  magic_mem[0] = n;
  magic_mem[1] = a0;
  magic_mem[2] = a1;
  magic_mem[3] = a2;
  magic_mem[4] = a3;
  magic_mem[5] = a4;
  magic_mem[6] = a5;
  magic_mem[7] = a6;

  htif_syscall((uintptr_t)magic_mem);

  long ret = magic_mem[0];

  spinlock_unlock(&lock);
  return ret;
}

/* Dump performance counter data for application */
static inline void hpm_print(void)
{
  static const char *label[] = {
    "cycles",
    "instret",
    "loads",
    "stores",
    "I$ miss",
    "D$ miss",
    "D$ release",
    "ITLB miss",
    "DTLB miss",
    "L2 TLB miss",
    "branches",
    "mispredicts",
    "load-use interlock",
    "I$ blocked",
    "D$ blocked",
  };
  _Static_assert((sizeof(label) / sizeof(char *)) == HPM_NCOUNTERS);
  struct hpm hpm;
  int i;

  hpm_read(&hpm);

  for (i = 0; i < HPM_NCOUNTERS; i++) {
    if (hpm.data[i] != 0) {
      printk("pk: %s: %lld\n", label[i], hpm.data[i] - hpm_off.data[i]);
    }
  }
}

void shutdown(int code)
{
  hpm_print();
  frontend_syscall(SYS_exit, code, 0, 0, 0, 0, 0, 0);
  while (1);
}

void copy_stat(struct stat* dest, struct frontend_stat* src)
{
  dest->st_dev = src->dev;
  dest->st_ino = src->ino;
  dest->st_mode = src->mode;
  dest->st_nlink = src->nlink;
  dest->st_uid = src->uid;
  dest->st_gid = src->gid;
  dest->st_rdev = src->rdev;
  dest->st_size = src->size;
  dest->st_blksize = src->blksize;
  dest->st_blocks = src->blocks;
  dest->st_atime = src->atime;
  dest->st_mtime = src->mtime;
  dest->st_ctime = src->ctime;
}
