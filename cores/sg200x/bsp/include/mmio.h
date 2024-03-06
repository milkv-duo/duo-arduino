/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MMIO_H__
#define __MMIO_H__

#include <stdint.h>

#define __raw_readb(a)         (*(volatile unsigned char *)(a))
#define __raw_readw(a)         (*(volatile unsigned short *)(a))
#define __raw_readl(a)         (*(volatile unsigned int *)(a))
#define __raw_readq(a)         (*(volatile unsigned long long *)(a))

#define __raw_writeb(v,a)      (*(volatile unsigned char *)(a) = (v))
#define __raw_writew(v,a)      (*(volatile unsigned short *)(a) = (v))
#define __raw_writel(v,a)      (*(volatile unsigned int *)(a) = (v))
#define __raw_writeq(v,a)      (*(volatile unsigned long long *)(a) = (v))

/*
 * I/O memory access primitives. Reads are ordered relative to any
 * following Normal memory access. Writes are ordered relative to any prior
 * Normal memory access.  The memory barriers here are necessary as RISC-V
 * doesn't define any ordering between the memory space and the I/O space.
 */
#define __io_br()   do {} while (0)
#define __io_ar(v)  __asm__ __volatile__ ("fence i,r" : : : "memory")
#define __io_bw()   __asm__ __volatile__ ("fence w,o" : : : "memory")
//#define __io_aw()   mmiowb_set_pending()
#define __io_aw()   do {} while (0)

#define readb(c)    ({ unsigned char  __v; __io_br(); __v = __raw_readb(c); __io_ar(__v); __v; })
#define readw(c)    ({ unsigned short __v; __io_br(); __v = __raw_readw(c); __io_ar(__v); __v; })
#define readl(c)    ({ unsigned int __v; __io_br(); __v = __raw_readl(c); __io_ar(__v); __v; })

#define writeb(v, c)    ({ __io_bw(); __raw_writeb((v), (c)); __io_aw(); })
#define writew(v, c)    ({ __io_bw(); __raw_writew((v), (c)); __io_aw(); })
#define writel(v, c)    ({ __io_bw(); __raw_writel((v), (c)); __io_aw(); })

#define readq(c)    ({ unsigned long __v; __io_br(); __v = __raw_readq(c); __io_ar(__v); __v; })
#define writeq(v, c)    ({ __io_bw(); __raw_writeq((v), (c)); __io_aw(); })

#define mmio_wr32 mmio_write_32
#define mmio_rd32 mmio_read_32

static inline void mmio_write_8(uintptr_t addr, uint8_t value)
{
    writeb(value, (void *) addr);
}

static inline uint8_t mmio_read_8(uintptr_t addr)
{
    return readb((void *) addr);
}

static inline void mmio_write_16(uintptr_t addr, uint16_t value)
{
    writew(value, (void *) addr);
}

static inline uint16_t mmio_read_16(uintptr_t addr)
{
    return readw((void *) addr);
}

static inline void mmio_write_32(uintptr_t addr, uint32_t value)
{
    writel(value, (void *) addr);
}

static inline uint32_t mmio_read_32(uintptr_t addr)
{
    return readl((void *) addr);
}

static inline void mmio_write_64(uintptr_t addr, uint64_t value)
{
    writeq(value, (void *) addr);
}

static inline uint64_t mmio_read_64(uintptr_t addr)
{
    return readq((void *) addr);
}

static inline void mmio_clrbits_32(uintptr_t addr, uint32_t clear)
{
    writel(readl((void *) addr) & ~clear , (void *) addr);
}

static inline void mmio_setbits_32(uintptr_t addr, uint32_t set)
{
    writel(readl((void *) addr) | set , (void *) addr);
}

static inline void mmio_clrsetbits_32(uintptr_t addr, uint32_t clear,
                                      uint32_t set)
{
    writel((readl((void *) addr) & ~clear) | set , (void *) addr);
}

/* from Linux usage */
#define ioremap(a, l)        (a)

#define _reg_read(addr)                   mmio_read_32((addr))
#define _reg_write(addr, data)            mmio_write_32((addr), (data))
#define _reg_write_mask(addr, mask, data) mmio_clrsetbits_32(addr, mask, data)

#define ioread8 readb
#define ioread16 readw
#define ioread32 readl
#define ioread64 readq

#define iowrite8 writeb
#define iowrite16 writew
#define iowrite32 writel
#define iowrite64 writeq

#endif /* __MMIO_H__ */
