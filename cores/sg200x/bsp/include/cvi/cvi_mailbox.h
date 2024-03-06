/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __CVI_MAILBOX_H__
#define __CVI_MAILBOX_H__

union cpu_mailbox_info_offset{
	char mbox_info;
	int reserved;
};

union cpu_mailbox_int_clr_offset{
	char mbox_int_clr;
	int reserved;
};

union cpu_mailbox_int_mask_offset{
	char mbox_int_mask;
	int reserved;
};

union cpu_mailbox_int_offset{
	char mbox_int;
	int reserved;
};

union cpu_mailbox_int_raw_offset{
	char mbox_int_raw;
	int reserved;
};

typedef union mailbox_set
{
	unsigned char mbox_set;
	unsigned int reserved;
} mailbox_set;

typedef union mailbox_status
{
	unsigned char mbox_status;
	unsigned int reserved;
} mailbox_status;

typedef union cpu_mailbox_status
{
	unsigned char mbox_status;
	unsigned int reserved;
} cpu_mailbox_status;

/* register mapping refers to mailbox user guide*/
struct cpu_mbox_int {
	union cpu_mailbox_int_clr_offset  cpu_mbox_int_clr;
	union cpu_mailbox_int_mask_offset cpu_mbox_int_mask;
	union cpu_mailbox_int_offset      cpu_mbox_int_int;
	union cpu_mailbox_int_raw_offset  cpu_mbox_int_raw;
};

struct mailbox_set_register {
	union  cpu_mailbox_info_offset cpu_mbox_en[4];      // 0x00, 0x04, 0x08, 0x0c
	struct cpu_mbox_int cpu_mbox_set[4];                // 0x10~0x1C, 0x20~0x2C, 0x30~0x3C, 0x40~0x4C
	int    reserved[4];                                 // 0x50~0x5C
	union  mailbox_set mbox_set;                        // 0x60
	union  mailbox_status mbox_status;                  // 0x64
	int    reserved2[2];                                // 0x68~0x6C
	union  cpu_mailbox_status cpu_mbox_status[4];       // 0x70
};

struct mailbox_done_register {
	union  cpu_mailbox_info_offset cpu_mbox_done_en[4];
	struct cpu_mbox_int cpu_mbox_done[4];
};

#define MAILBOX_MAX_NUM         0x0008
#define MAILBOX_DONE_OFFSET     0x0002
#define MAILBOX_CONTEXT_OFFSET  0x0400

#define MAILBOX_REG_BASE                        0x01900000
#define MAILBOX_REG_BUFF                        (MAILBOX_REG_BASE + 0x0400)
#define SPINLOCK_REG_BASE                       (MAILBOX_REG_BASE + 0x00c0)

#define RECEIVE_CPU  2 // c906L
#define SEND_TO_CPU1 1 // c906B
#define SEND_TO_CPU0 0 // ca53

#endif // end of__CVI_MAILBOX_H__

