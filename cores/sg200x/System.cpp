/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "common.h"
#include "csi_gpio_pin.h"
#include "csi_pin.h"
#include "csi_adc.h"
#include "csi_dma.h"
#include "HardwareSerial.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_SPI_DMA
static csi_dma_t dma_hdl;
#endif

void init(void)
{
    irq_init();
    tick_init();

#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.begin(115200);
#endif

#ifdef USE_SPI_DMA
    csi_dma_init(&dma_hdl, 0);
#endif

}

#ifdef __cplusplus
}
#endif
