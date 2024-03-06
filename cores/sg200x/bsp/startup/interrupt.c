/*
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

#include <stdint.h>
#include "common.h"
#include "csi_common.h"
#include "mmio.h"
#include "encoding.h"

#ifdef __cplusplus
extern "C" {
#endif
struct irq_chip {
    const char  *name;
    void (*irq_mask)(int irq_num);
    void (*irq_unmask)(int irq_num);
    int  (*irq_ack)(void);
    void (*irq_set_priority)(int irq_num, int priority);
    void (*irq_eoi)(int irq_num);
    void (*irq_set_threshold)(uint32_t threshold);
};

struct irq_action {
    irq_handler_t handler;
    unsigned int flags;
    unsigned int irqn;
    void *priv;
    char name[16];
};

#define IRQ_NUM_MAX   128
static struct irq_action g_irq_action[IRQ_NUM_MAX];
static struct irq_chip sirq_chip;

void disable_irq(unsigned int irqn)
{
    sirq_chip.irq_mask(irqn);
}

void enable_irq(unsigned int irqn)
{
    sirq_chip.irq_unmask(irqn);
}

static void plic_mask_irq(int irq_num)
{
    uint32_t mask = (irq_num);
    uint32_t value = 0;

    if (irq_num < 16) {
        pr_debug("mask irq_num is %d\n", irq_num);
        return;
    }
    value = mmio_read_32(PLIC_ENABLE1 + 4 * (mask / 32));
    value &= ~(0x1 << (mask % 32));
    mmio_write_32((PLIC_ENABLE1 + (mask / 32) * 4), value);

}

static void plic_unmask_irq(int irq_num)
{
    uint32_t mask = (irq_num);
    uint32_t value = 0;

    if (irq_num < 16) {
        pr_debug("unmask irq_num is %d\n", irq_num);
        return;
    }
    pr_debug("irq_num unmask=%d\n", mask);
    value = mmio_read_32(PLIC_ENABLE1 + 4 * (mask / 32));
    value |= (0x1 << (mask % 32));
    pr_debug("value=%x\n", value);
    mmio_write_32((PLIC_ENABLE1 + (mask / 32) * 4), value);
}

static int plic_ack_irq()
{
    return mmio_read_32(PLIC_CLAIM);
}

static void plic_eoi_irq(int irq_num)
{
    mmio_write_32(PLIC_CLAIM, irq_num);
}

static void plic_set_priority_irq(int irq_num, int priority)
{
    mmio_write_32((PLIC_PRIORITY0 + irq_num * 4), priority);
}

static void plic_set_threshold(uint32_t threshold)
{
    mmio_write_32((PLIC_THRESHOLD), threshold);
}

static struct irq_chip sirq_chip = {
    .name              = "RISCV PLIC",
    .irq_mask          = plic_mask_irq,
    .irq_unmask        = plic_unmask_irq,
    .irq_ack           = plic_ack_irq,
    .irq_set_priority  = plic_set_priority_irq,
    .irq_eoi           = plic_eoi_irq,
    .irq_set_threshold = plic_set_threshold,
};

void irq_init(void)
{
    int i;
    // clear interrupt enable
    write_csr(mie, 0);
    // clear interrupt pending
    write_csr(mip, 0);

    // Clean the setting of all IRQ
    for (i = 0; i < IRQ_NUM_MAX * 4; i = i + 4) {
        mmio_write_32(((uintptr_t) PLIC_PRIORITY0 + i), 0);
    }

    for (i = 0; i <= IRQ_NUM_MAX / 32; i++) {
        mmio_write_32((PLIC_PENDING1 + i * 4), 0);
        mmio_write_32((PLIC_ENABLE1 + i * 4), 0);
    }

    memset(g_irq_action, 0, sizeof(struct irq_action) * IRQ_NUM_MAX);
    sirq_chip.irq_set_threshold(0);

    set_csr(mstatus, MSTATUS_MIE);

    set_csr(mie, MIP_MEIP);
    //set_csr(mie, MIP_MTIP);
}

int request_irq(int irqn, irq_handler_t handler, unsigned long flags,
                const char *name, void *priv)
{
    pr_debug("request_irq irqn=%d\n handler=%lx name = %s\n", irqn, (long) handler, name);
    g_irq_action[irqn].handler = handler;
    if (name) {
        memcpy(g_irq_action[irqn].name, name, sizeof(g_irq_action[irqn].name));
        g_irq_action[irqn].name[sizeof(g_irq_action[irqn].name) - 1] = 0;
    }
    g_irq_action[irqn].irqn = irqn ; //- gic_data.hwirq_base;
    g_irq_action[irqn].flags = flags;
    g_irq_action[irqn].priv = priv;
    // set highest priority
    sirq_chip.irq_set_priority(irqn, 7);
    // unmask irq
    sirq_chip.irq_unmask(irqn);

    return 0;
}

int attach_irq(int irqn, irq_handler_t handler, unsigned long flags,
               const char *name, void *priv)
{
    g_irq_action[irqn].handler = handler;
    if (name) {
        memcpy(g_irq_action[irqn].name, name, sizeof(g_irq_action[irqn].name));
        g_irq_action[irqn].name[sizeof(g_irq_action[irqn].name) - 1] = 0;
    }
    g_irq_action[irqn].irqn = irqn ; //- gic_data.hwirq_base;
    g_irq_action[irqn].flags = flags;
    g_irq_action[irqn].priv = priv;
    // set highest priority
    sirq_chip.irq_set_priority(irqn, 7);
    return 0;
}

int detach_irq(int irqn)
{
    g_irq_action[irqn].handler = NULL;
    return 0;
}

void set_priority_irq(int irq_num, int priority)
{
    plic_set_priority_irq(irq_num, priority);
}

static void do_irq(void)
{
    int irqn;

    do {
        irqn = sirq_chip.irq_ack();
        pr_debug("trigger %d interrupt\n\r", irqn);
        if (g_irq_action[irqn].handler && irqn) {
            g_irq_action[irqn].handler(g_irq_action[irqn].irqn, g_irq_action[irqn].priv);
        } else if (irqn) {
        } else { // plic_claim =0
            break;
        }
        // clear plic pending
        sirq_chip.irq_eoi(irqn);
    } while (1);
    // clear external interrupt pending
    clear_csr(mip, MIP_MEIP);
}

void timer_interrupt()
{
    if (g_irq_action[IRQ_M_TIMER].handler) {
        g_irq_action[IRQ_M_TIMER].handler(g_irq_action[IRQ_M_TIMER].irqn, g_irq_action[IRQ_M_TIMER].priv);
    } else {
        pr_debug("timer interrupt handler is not set");
    }
    clear_csr(mip, MIP_MTIP);
}

uint64_t handle_trap(uint64_t scause, uint64_t epc, uint64_t *regs)
{
    uint64_t is_int;

    is_int = (scause & MCAUSE_INT);
    if ((is_int) && ((scause & MCAUSE_CAUSE) == IRQ_M_EXT))
        do_irq();
    else if ((is_int) && ((scause & MCAUSE_CAUSE) == IRQ_M_TIMER))
        timer_interrupt(0);     /* handle_m_timer_interrupt */

    return epc;
}

#ifdef __cplusplus
}
#endif
