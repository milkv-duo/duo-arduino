/* SPDX-License-Identifier: GPL-2.0 */

#include "stdint.h"
#include "csi_riscv_gcc.h"
#include "cvi_spinlock.h"

#define MAILBOX_REG_BASE 0x01900000
#define MAILBOX_REG_BUFF (MAILBOX_REG_BASE + 0x0400)
#define SPINLOCK_REG_BASE (MAILBOX_REG_BASE + 0x00c0)

static unsigned long reg_base = SPINLOCK_REG_BASE;

static unsigned char lockCount[SPIN_MAX + 1] = {0};

void cvi_spinlock_init()
{
}

void cvi_spinlock_deinit(void)
{
}

void spinlock_base(unsigned long mb_base)
{
	reg_base = mb_base;
}

static inline int hw_spin_trylock(hw_raw_spinlock_t *lock)
{
	writew(lock->locks, reg_base + sizeof(int) * lock->hw_field);
	if (readw(reg_base + sizeof(int) * lock->hw_field) == lock->locks)
		return MAILBOX_LOCK_SUCCESS;
	return MAILBOX_LOCK_FAILED;
}

int hw_spin_lock(hw_raw_spinlock_t *lock)
{
	unsigned long i;
	unsigned long loops = 1000000;
	hw_raw_spinlock_t _lock = {.hw_field = lock->hw_field, .locks = lock->locks};

	if (lockCount[lock->hw_field] == 0) {
		lockCount[lock->hw_field]++;
	}
	_lock.locks = (lockCount[lock->hw_field] << 8);
	lockCount[lock->hw_field]++;

	for (i = 0; i < loops; i++) {
		if (hw_spin_trylock(&_lock) == MAILBOX_LOCK_SUCCESS) {
			lock->locks = _lock.locks;
			return MAILBOX_LOCK_SUCCESS;
		}
		udelay(1);
	}

	return MAILBOX_LOCK_FAILED;
}

int _hw_raw_spin_lock_irqsave(hw_raw_spinlock_t *lock)
{
	int flag = 0;

	// save and disable irq
	flag = (__get_MSTATUS() & 8);
	__disable_irq();

	// lock
	if (hw_spin_lock(lock) == MAILBOX_LOCK_FAILED) {

		// if spinlock failed , restore irq
		if (flag) {
			__enable_irq();
		}

		return MAILBOX_LOCK_FAILED;
	}
	return flag;
}

void _hw_raw_spin_unlock_irqrestore(hw_raw_spinlock_t *lock, int flag)
{
	// unlock
	if (readw(reg_base + sizeof(int) * lock->hw_field) == lock->locks) {
		writew(lock->locks, reg_base + sizeof(int) * lock->hw_field);

		// restore irq
		if (flag) {
			__enable_irq();
		}
	}
}
