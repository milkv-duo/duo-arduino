/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "Arduino.h"
#include <string.h>
#include "mailbox.h"

#include "cvi_mailbox.h"

#define SOC_MBOX_INTERRUPT              0x3D

static volatile mailbox_set_register *mbox_reg;
static volatile mailbox_done_register *mbox_done_reg;
static volatile uint32_t *mailbox_context;

static void (*callbacks[NUM_MAILBOX_CHANNELS])(MailboxMsg);

MailboxMsg MBoxMsg[NUM_MAILBOX_CHANNELS];

void mailbox_register (int channel, void(*callback)(MailboxMsg))
{
    if (channel >= 0 && channel < NUM_MAILBOX_CHANNELS) {
        callbacks[channel] = callback;
    }
}

void mailbox_unregister (int channel)
{
    if (channel >= 0 && channel < NUM_MAILBOX_CHANNELS) {
        callbacks[channel] = 0;
    }
}

void mailbox_disable_receive (int channel)
{
    mbox_reg->cpu_mbox_set[SEND_TO_CPU1].cpu_mbox_int_mask.mbox_int_mask |= 1 << channel;
}

void mailbox_enable_receive (int channel)
{
    mbox_reg->cpu_mbox_set[SEND_TO_CPU1].cpu_mbox_int_mask.mbox_int_mask &= ~(1 << channel);
}

static void do_callback (int channel, MailboxMsg& msg)
{
    void (*cb)(MailboxMsg) = callbacks[channel];
    if (cb) {
        cb(msg);
    }
}

static void mailbox_read (int channel, MailboxMsg& msg)
{
    unsigned int i;

    /* Copy channel data into MailboxMsg object */
    msg.channel = channel;

    msg.data= (uint32_t *)(mailbox_context + (4 * channel));

    mbox_reg->cpu_mbox_set[RECEIVE_CPU].cpu_mbox_int_clr.mbox_int_clr = 1 << channel;
    mbox_reg->cpu_mbox_en[RECEIVE_CPU].mbox_info &= ~(1 << channel);
}

void mailbox_write (MailboxMsg& msg)
{

    uint32_t *ptr;

    ptr = (uint32_t *)(mailbox_context + (4 * msg.channel));

    ptr = static_cast<uint32_t*>(msg.data);

    // clear mailbox
    mbox_reg->cpu_mbox_set[SEND_TO_CPU1].cpu_mbox_int_clr.mbox_int_clr = (1 << msg.channel);
    // trigger mailbox valid to rtos
    mbox_reg->cpu_mbox_en[SEND_TO_CPU1].mbox_info |= (1 << msg.channel);
    mbox_reg->mbox_set.mbox_set = (1 << msg.channel);
}

static int mailbox_isr(int irqn, void *priv)
{
    int i;
    uint32_t sts;
    MailboxMsg msg;
    uint8_t valid_val;

    uint8_t set_val;
    set_val = mbox_reg->cpu_mbox_set[RECEIVE_CPU].cpu_mbox_int_int.mbox_int;

    for (i = 0; i < MAILBOX_MAX_NUM; i++)
    {
        valid_val = set_val & (1 << i);
        if (valid_val)
        {
            mailbox_read(i, msg);
            do_callback(i, msg);
        }
    }

    return 0;
}

static void mailbox_hardware_init (void)
{
}

static void mailbox_interrupts_init (bool master)
{
    disable_irq(SOC_MBOX_INTERRUPT);

    if (master)
        mailbox_hardware_init();

    attach_irq(SOC_MBOX_INTERRUPT, &mailbox_isr, 0, "mailbox int", NULL);
    enable_irq(SOC_MBOX_INTERRUPT);
}

void mailbox_init (bool master)
{
    int i;
    mbox_reg = (struct mailbox_set_register *)MAILBOX_REG_BASE;
    mbox_done_reg = (struct mailbox_done_register *)(MAILBOX_REG_BASE + 2);
    mailbox_context = (uint32_t *)(MAILBOX_REG_BUFF);

    for (i = 0; i < MAILBOX_MAX_NUM; i++)
        mailbox_context[i] = 0;

    memset(callbacks, 0, sizeof(callbacks));
    mailbox_interrupts_init(master);
}
