// See LICENSE for license details.

#ifndef _RISCV_HPM_H
#define _RISCV_HPM_H

#include <stdint.h>
#include "encoding.h"

#define HPM_NCOUNTERS 15

struct hpm {
  uint64_t data[HPM_NCOUNTERS];
};

extern struct hpm hpm_off;

#define HPM_EVENTSET_BITS       8
#define HPM_EVENTSET_MASK       ((1U << HPM_EVENTSET_BITS) - 1)
#define HPM_EVENT(event, set)   ((1U << ((event) + HPM_EVENTSET_BITS)) | ((set) & HPM_EVENTSET_MASK))

/* Initialize Rocket hardware performance monitor */
static inline void hpm_init(void)
{
  write_csr(mhpmevent3,  HPM_EVENT(1, 0)); // loads
  write_csr(mhpmevent4,  HPM_EVENT(2, 0)); // stores

  write_csr(mhpmevent5,  HPM_EVENT(0, 2)); // I$ miss
  write_csr(mhpmevent6,  HPM_EVENT(1, 2)); // D$ miss
  write_csr(mhpmevent7,  HPM_EVENT(2, 2)); // D$ release
  write_csr(mhpmevent8,  HPM_EVENT(3, 2)); // ITLB miss
  write_csr(mhpmevent9,  HPM_EVENT(4, 2)); // DTLB miss
  write_csr(mhpmevent10, HPM_EVENT(5, 2)); // L2 TLB miss

  write_csr(mhpmevent11, HPM_EVENT(6, 0)); // branches
  write_csr(mhpmevent12, HPM_EVENT(5, 1)); // branch misprediction

  write_csr(mhpmevent13, HPM_EVENT(0, 1)); // load-use interlock
  write_csr(mhpmevent14, HPM_EVENT(3, 1)); // I$ blocked
  write_csr(mhpmevent15, HPM_EVENT(4, 1)); // D$ blocked
}

void hpm_read(struct hpm *);

#endif /* _RISCV_HPM_H */
