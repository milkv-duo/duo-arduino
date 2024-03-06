/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include "common.h"

#define NUM_MAILBOX_CHANNELS 8

class MailboxMsg {
  public:
    uint32_t *data;
    int channel = 0;
};

void mailbox_init (bool master);
void mailbox_register (int channel, void(*callback)(MailboxMsg));
void mailbox_unregister (int channel);
void mailbox_enable_receive (int channel);
void mailbox_disable_receive (int channel);
void mailbox_write(MailboxMsg& msg);

#endif
