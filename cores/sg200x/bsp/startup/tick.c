/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

#include "encoding.h"
#include "csi_riscv_gcc.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

static volatile uint32_t csi_tick = 0U;
static volatile uint64_t last_time_ms = 0U;
static volatile uint64_t last_time_us = 0U;
static volatile uint64_t timer_init_value = 0U;

/**
  \brief  The data structure of the access Clint.
 */
typedef struct {
    __IOM uint32_t MSIP0;
    __IOM uint32_t MSIP1;
    __IOM uint32_t MSIP2;
    __IOM uint32_t MSIP3;
    uint32_t RESERVED0[(0x4004000 - 0x400000C) / 4 - 1];
    __IOM uint32_t MTIMECMPL0;
    __IOM uint32_t MTIMECMPH0;
    __IOM uint32_t MTIMECMPL1;
    __IOM uint32_t MTIMECMPH1;
    __IOM uint32_t MTIMECMPL2;
    __IOM uint32_t MTIMECMPH2;
    __IOM uint32_t MTIMECMPL3;
    __IOM uint32_t MTIMECMPH3;
    uint32_t RESERVED1[(0x400C000 - 0x400401C) / 4 - 1];
    __IOM uint32_t SSIP0;
    __IOM uint32_t SSIP1;
    __IOM uint32_t SSIP2;
    __IOM uint32_t SSIP3;
    uint32_t RESERVED2[(0x400D000 - 0x400C00C) / 4 - 1];
    __IOM uint32_t STIMECMPL0;
    __IOM uint32_t STIMECMPH0;
    __IOM uint32_t STIMECMPL1;
    __IOM uint32_t STIMECMPH1;
    __IOM uint32_t STIMECMPL2;
    __IOM uint32_t STIMECMPH2;
    __IOM uint32_t STIMECMPL3;
    __IOM uint32_t STIMECMPH3;
} CLINT_Type;

/**
  \brief   Get MTIME
  \details Returns the content of the MTIME Register.
  \return               MTIME Register value
  */
__ALWAYS_STATIC_INLINE uint64_t __get_MTIME(void)
{
    uint64_t result;

    __ASM volatile("rdtime %0" : "=r"(result));
    //__ASM volatile("csrr %0, 0xc01" : "=r"(result));
    return (result);
}

static inline uint32_t csi_clint_config(CLINT_Type *clint, uint32_t ticks, int32_t IRQn)
{
#ifdef __QEMU_RUN
    uint64_t value = (((uint64_t)clint->MTIMECMPH0) << 32) + (uint64_t)clint->MTIMECMPL0;

    value = value + (uint64_t)ticks;
    clint->MTIMECMPH0 = (uint32_t)(value >> 32);
    clint->MTIMECMPL0 = (uint32_t)value;
#else
    uint64_t value = (((uint64_t)clint->MTIMECMPH0) << 32) + (uint64_t)clint->MTIMECMPL0;

    if ((value != 0) && (value != 0xffffffffffffffff)) {
        value = value + (uint64_t)ticks;
    } else {
        value = __get_MTIME() + ticks;
    }
    clint->MTIMECMPH0 = (uint32_t)(value >> 32);
    clint->MTIMECMPL0 = (uint32_t)value;
#endif /*__QEMU_RUN*/

    return (0UL);
}

void csi_tick_increase(void)
{
    csi_tick++;
}

static void tick_irq_handler(int irqn, void *priv)
{
    csi_tick_increase();
    csi_clint_config((CLINT_Type *)priv, clockCyclesPerTick(), IRQ_M_TIMER);
}

void tick_init(void)
{
    CLINT_Type *clint = (CLINT_Type *)CLINT_BASE;

    csi_tick = 0U;

    timer_init_value = csi_clint_get_value();
    attach_irq(IRQ_M_TIMER, &tick_irq_handler, 0, "M TIMER int", clint);
    set_priority_irq(IRQ_M_TIMER, 31U);
    clint->MTIMECMPH0 = 0;
    clint->MTIMECMPL0 = 0;

    csi_clint_config(clint, clockCyclesPerTick(), IRQ_M_TIMER);

    set_csr(mie, MIP_MTIP);
}

void csi_tick_uninit(void)
{
    disable_irq(IRQ_M_TIMER);
}

uint32_t csi_tick_get(void)
{
    return csi_tick;
}

uint64_t millis(void)
{
    uint64_t time;
 
    time = ((csi_clint_get_value() - timer_init_value) * 1000U / (uint64_t)F_CPU);
    last_time_ms = time;
    return time;
}

uint64_t micros(void)
{
    uint64_t time;

    time = (csi_clint_get_value() - timer_init_value) * 1000U * 1000U / F_CPU;
    last_time_us = time;
    return time;
}

#ifdef __cplusplus
} // extern "C"
#endif
