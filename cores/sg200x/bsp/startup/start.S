/*
 * FreeRTOS V202111.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://www.github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */

#define REGSIZE     8
#define REGSHIFT    3
#define LOAD        ld
#define STOR        sd

.org 0
    .section .vectors, "ax"
    .globl Reset_Handler
    .type Reset_Handler,@function
Reset_Handler:
    .cfi_startproc
    .cfi_undefined ra
.option push
.option norelax
//    la  gp, __global_pointer$
.option pop

    // Continue primary hart
    csrr a0, mhartid
    // li   a1, PRIM_HART
    // bne  a0, a1, secondary

    li x1, 0
    li x2, 0
    li x3, 0
    li x4, 0
    li x5, 0
    li x6, 0
    li x7, 0
    li x8, 0
    li x9, 0
    li x10, 0
    li x11, 0
    li x12, 0
    li x13, 0
    li x14, 0
    li x15, 0
    li x16, 0
    li x17, 0
    li x18, 0
    li x19, 0
    li x20, 0
    li x21, 0
    li x22, 0
    li x23, 0
    li x24, 0
    li x25, 0
    li x26, 0
    li x27, 0
    li x28, 0
    li x29, 0
    li x30, 0
    li x31, 0

    csrw mscratch, x0

    csrw mie, x0

    # write mtvec and make sure it sticks
    la t0, trap_vector
    csrw mtvec, t0

    // enable interrupt
    li x3, 0x880
    csrw mie, x3

    # enable fp
    li x3, 0x1 << 13
    csrs mstatus, x3

    // Primary hart
    la sp, _stack_top

2:

    // Clear bss section
    la a0, _bss
    la a1, _ebss
    bgeu a0, a1, 2f
1:
    // reduce branch time, be sure about bss alignment in linker script
    STOR zero, 0x00 (a0)
    STOR zero, 0x08 (a0)
    STOR zero, 0x10 (a0)
    STOR zero, 0x18 (a0)
    STOR zero, 0x20 (a0)
    STOR zero, 0x28 (a0)
    STOR zero, 0x30 (a0)
    STOR zero, 0x38 (a0)
    addi a0, a0, REGSIZE * 8
    bltu a0, a1, 1b
2:

    // argc, argv, envp is 0
    li  a0, 0
    li  a1, 0
    li  a2, 0
    jal main
1:
    wfi
    j 1b

secondary:
    // TODO: Multicore is not supported
    wfi
    j secondary
    .cfi_endproc

trap_vector:
  j trap_entry
  // never reach here
  j die

/*
 * trap entry
 */
trap_entry:
    addi sp, sp, -35 * REGSIZE
    STOR x1, 1 * REGSIZE(sp)
    STOR x2, 2 * REGSIZE(sp)
    STOR x3, 3 * REGSIZE(sp)
    STOR x4, 4 * REGSIZE(sp)
    STOR x5, 5 * REGSIZE(sp)
    STOR x6, 6 * REGSIZE(sp)
    STOR x7, 7 * REGSIZE(sp)
    STOR x8, 8 * REGSIZE(sp)
    STOR x9, 9 * REGSIZE(sp)
    STOR x10, 10 * REGSIZE(sp)
    STOR x11, 11 * REGSIZE(sp)
    STOR x12, 12 * REGSIZE(sp)
    STOR x13, 13 * REGSIZE(sp)
    STOR x14, 14 * REGSIZE(sp)
    STOR x15, 15 * REGSIZE(sp)
    STOR x16, 16 * REGSIZE(sp)
    STOR x17, 17 * REGSIZE(sp)
    STOR x18, 18 * REGSIZE(sp)
    STOR x19, 19 * REGSIZE(sp)
    STOR x20, 20 * REGSIZE(sp)
    STOR x21, 21 * REGSIZE(sp)
    STOR x22, 22 * REGSIZE(sp)
    STOR x23, 23 * REGSIZE(sp)
    STOR x24, 24 * REGSIZE(sp)
    STOR x25, 25 * REGSIZE(sp)
    STOR x26, 26 * REGSIZE(sp)
    STOR x27, 27 * REGSIZE(sp)
    STOR x28, 28 * REGSIZE(sp)
    STOR x29, 29 * REGSIZE(sp)
    STOR x30, 30 * REGSIZE(sp)
    STOR x31, 31 * REGSIZE(sp)
    csrr a1, mepc
    STOR a1,   0 * REGSIZE(sp)
    csrr t0, mstatus
    STOR t0,  32 * REGSIZE(sp)
    csrr t1, mbadaddr
    STOR t1,  33 * REGSIZE(sp)
    csrr a0, mcause
    STOR a0,  34 * REGSIZE(sp)
    mv a2, sp
    jal handle_trap

    LOAD t0, 32 * REGSIZE(sp)
    LOAD a1,  0 * REGSIZE(sp)
    csrw mstatus, t0
    csrw mepc, a1

    LOAD x1, 1 * REGSIZE(sp)
    LOAD x2, 2 * REGSIZE(sp)
    LOAD x3, 3 * REGSIZE(sp)
    LOAD x4, 4 * REGSIZE(sp)
    LOAD x5, 5 * REGSIZE(sp)
    LOAD x6, 6 * REGSIZE(sp)
    LOAD x7, 7 * REGSIZE(sp)
    LOAD x8, 8 * REGSIZE(sp)
    LOAD x9, 9 * REGSIZE(sp)
    LOAD x10, 10 * REGSIZE(sp)
    LOAD x11, 11 * REGSIZE(sp)
    LOAD x12, 12 * REGSIZE(sp)
    LOAD x13, 13 * REGSIZE(sp)
    LOAD x14, 14 * REGSIZE(sp)
    LOAD x15, 15 * REGSIZE(sp)
    LOAD x16, 16 * REGSIZE(sp)
    LOAD x17, 17 * REGSIZE(sp)
    LOAD x18, 18 * REGSIZE(sp)
    LOAD x19, 19 * REGSIZE(sp)
    LOAD x20, 20 * REGSIZE(sp)
    LOAD x21, 21 * REGSIZE(sp)
    LOAD x22, 22 * REGSIZE(sp)
    LOAD x23, 23 * REGSIZE(sp)
    LOAD x24, 24 * REGSIZE(sp)
    LOAD x25, 25 * REGSIZE(sp)
    LOAD x26, 26 * REGSIZE(sp)
    LOAD x27, 27 * REGSIZE(sp)
    LOAD x28, 28 * REGSIZE(sp)
    LOAD x29, 29 * REGSIZE(sp)
    LOAD x30, 30 * REGSIZE(sp)
    LOAD x31, 31 * REGSIZE(sp)
    addi sp, sp, 35 * REGSIZE
    mret

die:
1:
  j 1b
