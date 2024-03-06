/* SPDX-License-Identifier: GPL-2.0 */

#include "common.h"
#include "cvi_mailbox.h"
#include "cvi_spinlock.h"

DEFINE_CVI_SPINLOCK(mailbox_lock, SPIN_MBOX);

void mailbox_init();
// void mailbox_register (int channel, void(*callback)(MailboxMsg));
// void mailbox_unregister (int channel);
void mailbox_enable_receive(int channel);
void mailbox_disable_receive(int channel);
void mailbox_write(int channel, void *msg);

int send_to_cpu = SEND_TO_CPU1;

unsigned int reg_base = MAILBOX_REG_BASE;
volatile struct mailbox_set_register *mbox_reg;
volatile struct mailbox_done_register *mbox_done_reg;
volatile unsigned int *mailbox_context;

void mailbox_init()
{
    int i;
    mbox_reg = (struct mailbox_set_register *)reg_base;
    mbox_done_reg = (struct mailbox_done_register *)(reg_base + 2);
    mailbox_context = (unsigned int *)(MAILBOX_REG_BUFF);

    for (i = 0; i < MAILBOX_MAX_NUM; i++)
        mailbox_context[i] = 0;
}

void mailbox_write(int channel, void *msg)
{
    int valid;
    int flags;
    uint32_t *ptr;

    ptr = (uint32_t *)mailbox_context + (4 * channel);

    drv_spin_lock_irqsave(&mailbox_lock, flags);
    if (flags == MAILBOX_LOCK_FAILED) {
        return;
    }

    *ptr = (uint32_t *)msg;

    // clear mailbox
    mbox_reg->cpu_mbox_set[send_to_cpu].cpu_mbox_int_clr.mbox_int_clr = (1 << valid);
    // trigger mailbox valid to rtos
    mbox_reg->cpu_mbox_en[send_to_cpu].mbox_info |= (1 << valid);
    mbox_reg->mbox_set.mbox_set = (1 << valid);

    drv_spin_unlock_irqrestore(&mailbox_lock, flags);
    if (valid >= MAILBOX_MAX_NUM) {
        printf("No valid mailbox is available\n");
        return -1;
    }
}

void mailbox_isr(void)
{
    unsigned char set_val;
    int i;
    set_val = mbox_reg->cpu_mbox_set[RECEIVE_CPU].cpu_mbox_int_int.mbox_int;
    unsigned char valid_val;

    uint32_t *msg;

    if (set_val) {
        for (i = 0; i < MAILBOX_MAX_NUM; i++) {
            valid_val = set_val & (1 << i);

            if (valid_val) {
                msg = (uint32_t *)mailbox_context + (4 * i);

                /* mailbox buffer context is send from linux, clear mailbox interrupt */
                mbox_reg->cpu_mbox_set[RECEIVE_CPU].cpu_mbox_int_clr.mbox_int_clr = valid_val;
                // need to disable enable bit
                mbox_reg->cpu_mbox_en[RECEIVE_CPU].mbox_info &= ~valid_val;
            }
        }
    }
}
