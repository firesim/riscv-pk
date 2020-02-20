#include "hpm.h"

struct hpm hpm_off;

/* Snapshot performance counter data */
void hpm_read(struct hpm *hpm)
{
#if (__riscv_xlen == 32)
#error "HPM unsupported for RV32"
#endif

  hpm->data[0] = read_csr(cycle);
  hpm->data[1] = read_csr(instret);
  hpm->data[2] = read_csr(hpmcounter3);
  hpm->data[3] = read_csr(hpmcounter4);
  hpm->data[4] = read_csr(hpmcounter5);
  hpm->data[5] = read_csr(hpmcounter6);
  hpm->data[6] = read_csr(hpmcounter7);
  hpm->data[7] = read_csr(hpmcounter8);
  hpm->data[8] = read_csr(hpmcounter9);
  hpm->data[9] = read_csr(hpmcounter10);
  hpm->data[10] = read_csr(hpmcounter11);
  hpm->data[11] = read_csr(hpmcounter12);
  hpm->data[12] = read_csr(hpmcounter13);
  hpm->data[13] = read_csr(hpmcounter14);
  hpm->data[14] = read_csr(hpmcounter15);
}
