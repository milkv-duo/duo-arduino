/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#ifndef _CSI_RV64_GCC_H_
#define _CSI_RV64_GCC_H_

#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
  \brief   Get MSTATUS
  \details Returns the content of the MSTATUS Register.
  \return               MSTATUS Register value
 */
__ALWAYS_STATIC_INLINE uint64_t __get_MSTATUS(void)
{
    uint64_t result;

    __ASM volatile("csrr %0, mstatus" : "=r"(result));
    return (result);
}

/**
  \brief   Set MSTATUS
  \details Writes the given value to the MSTATUS Register.
  \param [in]    mstatus  MSTATUS Register value to set
 */
__ALWAYS_STATIC_INLINE void __set_MSTATUS(uint64_t mstatus)
{
    __ASM volatile("csrw mstatus, %0" : : "r"(mstatus));
}

/**
  \brief   Disable IRQ Interrupts
  \details Disables IRQ interrupts by clearing the IE-bit in the PSR.
  Can only be executed in Privileged modes.
 */
__ALWAYS_STATIC_INLINE void __disable_irq(void)
{
    __ASM volatile("csrc mstatus, 8");
}

/* ##################################    IRQ Functions  ############################################ */

/**
  \brief   Save the Irq context
  \details save the psr result before disable irq.
 */
__STATIC_INLINE uint64_t csi_irq_save(void)
{
    uint64_t result;
    result = __get_MSTATUS();
    __disable_irq();
    return (result);
}

/**
  \brief   Restore the Irq context
  \details restore saved primask state.
  \param [in]      irq_state  psr irq state.
 */
__STATIC_INLINE void csi_irq_restore(uint64_t irq_state)
{
    __set_MSTATUS(irq_state);
}

/*@} end of IRQ Functions */

/**
  \brief   get CORE timer counter value
  \return          CORE timer counter value.
 */
static inline uint64_t csi_clint_get_value(void)
{
    uint64_t result;
    __asm volatile("csrr %0, 0xc01" : "=r"(result));
    return result;
}

#ifdef __cplusplus
}
#endif

#endif /* _CSI_RV64_GCC_H_ */
