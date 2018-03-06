#include "encoding.h"

  .section .rodata
  .globl logo
logo:
  .incbin SM_LOGO_FILE
  .byte 0
