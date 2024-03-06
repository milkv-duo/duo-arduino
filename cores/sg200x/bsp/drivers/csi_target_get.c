/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "csi_pin.h"
#include "hal_dma.h"

const csi_perip_info_t g_soc_info[] = {
    {DW_GPIO0_BASE,            GPIO0_INTR_FLAG,          0,    DEV_DW_GPIO_TAG},
    {DW_GPIO1_BASE,            GPIO1_INTR_FLAG,          1,    DEV_DW_GPIO_TAG},
    {DW_GPIO2_BASE,            GPIO2_INTR_FLAG,          2,    DEV_DW_GPIO_TAG},
    {DW_GPIO3_BASE,            GPIO3_INTR_FLAG,          3,    DEV_DW_GPIO_TAG},
    {DW_RTC_GPIO_BASE,         GPIO_INT,                 4,    DEV_DW_GPIO_TAG},
    {DW_UART0_BASE,            UART0_INTR,               0,    DEV_DW_UART_TAG},
    {DW_UART1_BASE,            UART1_INTR,               1,    DEV_DW_UART_TAG},
    {DW_UART2_BASE,            UART2_INTR,               2,    DEV_DW_UART_TAG},
    {DW_UART3_BASE,            UART3_INTR,               3,    DEV_DW_UART_TAG},
    {DW_UART4_BASE,            UART4_INTR,               4,    DEV_DW_UART_TAG},
    {DW_DMA_BASE,              DW_DMA_IRQn,              0,    DEV_DW_DMA_TAG},
    {DW_TIMER0_BASE,           TIMER_INTR_0,             0,    DEV_DW_TIMER_TAG},
    {DW_TIMER1_BASE,           TIMER_INTR_1,             1,    DEV_DW_TIMER_TAG},
    {DW_TIMER2_BASE,           TIMER_INTR_2,             2,    DEV_DW_TIMER_TAG},
    {DW_TIMER3_BASE,           TIMER_INTR_3,             3,    DEV_DW_TIMER_TAG},
    {DW_TIMER4_BASE,           TIMER_INTR_4,             4,    DEV_DW_TIMER_TAG},
    {DW_TIMER5_BASE,           TIMER_INTR_5,             5,    DEV_DW_TIMER_TAG},
    {DW_TIMER6_BASE,           TIMER_INTR_6,             6,    DEV_DW_TIMER_TAG},
    {DW_TIMER7_BASE,           TIMER_INTR_7,             7,    DEV_DW_TIMER_TAG},
    {DW_IIC0_BASE,             I2C0_INTR,                0,    DEV_DW_IIC_TAG},
    {DW_IIC1_BASE,             I2C1_INTR,                1,    DEV_DW_IIC_TAG},
    {DW_IIC2_BASE,             I2C2_INTR,                2,    DEV_DW_IIC_TAG},
    {DW_IIC3_BASE,             I2C3_INTR,                3,    DEV_DW_IIC_TAG},
    {DW_IIC4_BASE,             I2C4_INTR,                4,    DEV_DW_IIC_TAG},
    {DW_SPI0_BASE,             SPI_0_SSI_INTR,           0,    DEV_DW_SPI_TAG},
    {DW_SPI1_BASE,             SPI_1_SSI_INTR,           1,    DEV_DW_SPI_TAG},
    {DW_SPI2_BASE,             SPI_2_SSI_INTR,           2,    DEV_DW_SPI_TAG},
    {DW_SPI3_BASE,             SPI_3_SSI_INTR,           3,    DEV_DW_SPI_TAG},
    {CVI_PWM0_BASE,            0,                        0,    DEV_CVI_PWM_TAG},
    {CVI_PWM1_BASE,            0,                        1,    DEV_CVI_PWM_TAG},
    {CVI_PWM2_BASE,            0,                        2,    DEV_CVI_PWM_TAG},
    {CVI_PWM3_BASE,            0,                        3,    DEV_CVI_PWM_TAG},
    {ADC_BASE,                 SPACC_IRQ,                0,    DEV_CVI_ADC_TAG},
    {DW_SDIO0_BASE,            SD0_INTR,                 0,    DEV_DW_SDMMC_TAG,},
    {DW_SDIO1_BASE,            SD1_INTR,                 1,    DEV_DW_SDMMC_TAG,},
    {CVI_WDT0_BASE,            WDT0_INTR,                0,    DEV_DW_WDT_TAG},
    {CVI_WDT1_BASE,            WDT1_INTR,                1,    DEV_DW_WDT_TAG},
    {CVI_WDT2_BASE,            WDT2_INTR,                2,    DEV_DW_WDT_TAG},
    {CVI_RTC_WDT_BASE,         0,                        3,    DEV_DW_WDT_TAG},
    {CVI_TEMPSEN_BASE,         TEMPSEN_IRQ_O,            0,    DEV_DW_TEMPSEN_TAG},
    {0, 0, 0, 0}
};

csi_error_t target_get(csi_dev_tag_t dev_tag, uint32_t idx, csi_dev_t *dev)
{
    csi_error_t ret = CSI_OK;
    csi_perip_info_t *info;

    if (dev == NULL) {
        ret = CSI_ERROR;
    }

    ///< 使用包含外设基地址，外设中断号，外设设备号，外设设备类型成员的结构体数组变量初始化info
    info = (csi_perip_info_t *)&g_soc_info;

    ///< 获取相应的设备类型和设备号
    while (info->reg_base) {
        if ((info->dev_tag == (uint16_t)dev_tag) && (info->idx == (uint8_t)idx)) {
            break;
        }

        info++;
    }

    ///< 初始化设备的统一句柄：基地址，中断号，设备号，设备类型
    if (info->reg_base == 0U) {
        ret = CSI_ERROR;
    }

    if (ret != CSI_ERROR) {
        dev->reg_base = info->reg_base;
        dev->irq_num = info->irq_num;
        dev->idx = info->idx;
        dev->dev_tag = (uint16_t)dev_tag;
    }

    return ret;
}

uint32_t target_pin_to_devidx(pin_name_t pin_name, const csi_pinmap_t *pinmap)
{
    const csi_pinmap_t *map = pinmap;
    uint32_t ret = 0xFFFFFFFFU;

    while ((uint32_t)map->pin_name != 0xFFFFFFFFU) {
        if ((map->pin_name == pin_name) && (csi_pin_get_mux(pin_name) == map->pin_func)) {
            ret = map->idx;
            break;
        }

        map++;
    }

    return ret;
}

uint32_t target_pin_to_channel(pin_name_t pin_name, const csi_pinmap_t *pinmap)
{
    const csi_pinmap_t *map = pinmap;
    uint32_t ret = 0xFFFFFFFFU;

    while ((uint32_t)map->pin_name != 0xFFFFFFFFU) {
        if (map->pin_name == pin_name) {
            ret = (uint32_t)map->channel;
            break;
        }

        map++;
    }

    return ret;
}

pin_name_t target_gpio_to_pin(uint8_t gpio_idx, uint8_t channel, const csi_pinmap_t *pinmap)
{
    const csi_pinmap_t *map = pinmap;
    pin_name_t ret = (pin_name_t)0xFFU;

    while ((uint32_t)map->pin_name != 0xFFFFFFFFU) {
        if ((map->idx == gpio_idx) && (map->channel == channel)) {
            ret = map->pin_name;
            break;
        }

        map++;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif
