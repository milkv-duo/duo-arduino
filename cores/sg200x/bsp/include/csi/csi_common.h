/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

#ifndef _DRV_COMMON_H_
#define _DRV_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "variant.h"

// #include "pinctrl-mars.h"

#ifdef DEBUG_SERIAL
#define pr_err pr_printf
//#define DEBUG
#ifdef DEBUG
#define pr_debug pr_printf
#define pr_warn pr_printf
#else
#define pr_debug(x, args...)
#define pr_warn(x, args...)
#endif
#else
#define pr_err(x, args...)
#define pr_debug(x, args...)
#define pr_warn(x, args...)
#endif

void pr_printf(const char *format, ...);
void inv_dcache_range(uintptr_t start, size_t size);
void clean_dcache_range(uintptr_t start, size_t size);
void flush_dcache_range(uintptr_t start, size_t size);

#define     __IM     volatile const       /*! Defines 'read only' structure member permissions */
#define     __OM     volatile             /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile             /*! Defines 'read / write' structure member permissions */

#ifdef CONFIG_DEBUG_MODE
#define CSI_ASSERT(expr)                            \
        do {                                        \
            if ((long)expr == (long)NULL) {         \
                pr_err("PROGRAM ASSERT\n");         \
                while (1);                           \
            }                                       \
        } while (0);
#else
#define CSI_ASSERT(expr)        ((void)0U)
#endif

#ifndef CONFIG_PARAM_NOT_CHECK
#define CSI_PARAM_CHK(para, err)                        \
    do                                                  \
    {                                                   \
        if ((unsigned long)para == (unsigned long)NULL) \
        {                                               \
            pr_err("check param fail");                 \
            return (err);                               \
        }                                               \
    } while (0)

#define CSI_PARAM_CHK_NORETVAL(para)                    \
    do                                                  \
    {                                                   \
        if ((unsigned long)para == (unsigned long)NULL) \
        {                                               \
            pr_err("check param noret fail");           \
            return;                                     \
        }                                               \
    } while (0)
#else
#define CSI_PARAM_CHK(para, err)
#define CSI_PARAM_CHK_NORETVAL(para)
#endif


typedef enum {
    DEV_BLANK_TAG = 0U,
    DEV_DW_UART_TAG,
    DEV_DW_DMA_TAG,
    DEV_DW_GPIO_TAG,
    DEV_DW_IIC_TAG,
    DEV_DW_MAC_TAG,
    DEV_DW_QSPI_TAG,
    DEV_DW_SDMMC_TAG,
    DEV_DW_SDHCI_TAG,
    DEV_DW_SPI_TAG,
    DEV_DW_TIMER_TAG,
    DEV_DW_WDT_TAG,
    DEV_WJ_ADC_TAG,
    DEV_CVI_ADC_TAG,
    DEV_WJ_AES_TAG,
    DEV_WJ_CODEC_TAG,
    DEV_WJ_CRC_TAG,
    DEV_WJ_DMA_TAG,
    DEV_WJ_EFLASH_TAG,
    DEV_WJ_EFUSE_TAG,
    DEV_WJ_ETB_TAG,
    DEV_WJ_FFT_TAG,
    DEV_WJ_I2S_TAG,
    DEV_WJ_MBOX_TAG,
    DEV_WJ_PADREG_TAG,
    DEV_WJ_PDM_TAG,
    DEV_WJ_PINMUX_TAG,
    DEV_WJ_PMU_TAG,
    DEV_WJ_PWM_TAG,
    DEV_CVI_PWM_TAG,
    DEV_WJ_RNG_TAG,
    DEV_WJ_ROM_TAG,
    DEV_WJ_RSA_TAG,
    DEV_WJ_RTC_TAG,
    DEV_CVI_RTC_TAG,
    DEV_WJ_SASC_TAG,
    DEV_WJ_SHA_TAG,
    DEV_WJ_SPDIF_TAG,
    DEV_WJ_SPIDF_TAG,
    DEV_WJ_TDM_TAG,
    DEV_WJ_TIPC_TAG,
    DEV_WJ_USB_TAG,
    DEV_WJ_USI_TAG,
    DEV_WJ_VAD_TAG,
    DEV_CD_QSPI_TAG,
    DEV_DCD_ISO7816_TAG,
    DEV_OSR_RNG_TAG,
    DEV_QX_RTC_TAG,
    DEV_RCHBAND_CODEC_TAG,
    DEV_CMSDK_UART_TAG,
    DEV_RAMBUS_150B_PKA_TAG,
    DEV_RAMBUS_150B_TRNG_TAG,
    DEV_RAMBUS_120SI_TAG,
    DEV_RAMBUS_120SII_TAG,
    DEV_RAMBUS_120SIII_TAG,
	DEV_WJ_AVFS_TAG,
    DEV_WJ_BMU_TAG,
	DEV_DW_TEMPSEN_TAG
} csi_dev_tag_t;

typedef enum {
    CSI_OK          =  0,
    CSI_ERROR       = -1,
    CSI_BUSY        = -2,
    CSI_TIMEOUT     = -3,
    CSI_UNSUPPORTED = -4
} csi_error_t;

typedef struct {
   uint8_t    readable;
   uint8_t    writeable;
   uint8_t    error;
} csi_state_t;

typedef struct csi_dev csi_dev_t;

struct csi_dev {
    unsigned long reg_base;
    uint8_t       irq_num;
    uint8_t       idx;
    uint16_t      dev_tag;
    void          (*irq_handler)(uint32_t irqn, void *arg);
    void          *arg;
#ifdef CONFIG_PM
    csi_pm_dev_t  pm_dev;
#endif
};

#define HANDLE_REG_BASE(handle)     (handle->dev.reg_base)
#define HANDLE_IRQ_NUM(handle)      (handle->dev.irq_num)
#define HANDLE_DEV_IDX(handle)      (handle->dev.idx)
#define HANDLE_IRQ_HANDLER(handle)  (handle->dev.irq_handler)

typedef enum {
    CVI_OK          =  0,
    CVI_ERROR       = -1,
    CVI_BUSY        = -2,
    CVI_TIMEOUT     = -3,
    CVI_UNSUPPORTED = -4
} cvi_error_t;

typedef struct {
    unsigned long reg_base;
    uint8_t       irq_num;
    uint8_t       idx;
    void          (*irq_handler)(uint32_t irqn,void *arg);
    void           *arg;
} cvi_dev_t;

#define GET_DEV_REG_BASE(handle)     (handle->dev.reg_base)
#define GET_DEV_IDX(handle)      (handle->dev.idx)
#define GET_DEV_IRQ_NUM(handle)      (handle->dev.irq_num)
#define GET_DEV_IRQ_HANDLER(handle)  (handle->dev.irq_handler)

csi_error_t target_get(csi_dev_tag_t dev_tag, uint32_t idx, csi_dev_t *dev);

typedef struct {
    unsigned long    reg_base;
    uint8_t          irq_num;
    uint8_t          idx;
    uint16_t         dev_tag;
} csi_perip_info_t;

#ifdef __cplusplus
}
#endif

#endif /* _DRV_COMMON_H_ */
