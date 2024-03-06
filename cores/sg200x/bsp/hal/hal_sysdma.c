/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "hal_sysdma.h"

static struct dma_remap_item remap_table[] = {
    {CVI_UART2_TX, 1},    //13
    {CVI_UART2_RX, 0},    //13
    {CVI_I2S3_TX, 2},    //13
    {CVI_I2S0_RX, 3},    //13
    {CVI_SPI2_RX, 4},
    {CVI_SPI2_TX, 5},
    {CVI_I2S1_RX, 6},
    {CVI_SPI_NAND, 7}

};

static uint32_t remap0_val;
static uint32_t remap1_val;

static void dma_hs_remap(request_index_t hs_id, uint8_t channel_id)
{
    if (channel_id < 4) {
        remap0_val |= hs_id << (channel_id << 3);
    } else {
        channel_id -= 4;
        remap1_val |= hs_id << (channel_id << 3);
    }
}

void hal_dma_hs_remap_init(void)
{
    int table_size = sizeof(remap_table) / sizeof(struct dma_remap_item);

    for (int i = 0; i < table_size; i++) {
        dma_hs_remap(remap_table[i].hs_id, remap_table[i].channel_id);
    }

    remap0_val |= 1 << 31;
    remap1_val |= 1 << 31;

    *((volatile uint32_t *)REG_SDMA_DMA_CH_REMAP0) = remap0_val;
    *((volatile uint32_t *)REG_SDMA_DMA_CH_REMAP1) = remap1_val;

    //printf("remap0=%x\r\n", *((volatile uint32_t *)REG_SDMA_DMA_CH_REMAP0));
    //printf("remap1=%x\r\n", *((volatile uint32_t *)REG_SDMA_DMA_CH_REMAP1));
}
