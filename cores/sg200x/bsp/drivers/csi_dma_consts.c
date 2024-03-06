/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "csi_dma.h"

const uint8_t g_dma_chnum[] = {16};

/* DMA handshake number */
/* The member of uart_tx_hs_num is the handshake number for ETB */
const uint16_t uart_tx_hs_num[] = {19};
const uint16_t uart_rx_hs_num[] = {18};
const uint16_t iic_tx_hs_num[] = {21, 23};
const uint16_t iic_rx_hs_num[] = {20, 22};
const uint16_t spi_tx_hs_num[] = {17, 19, 21, 23};  //TOFIX
const uint16_t spi_rx_hs_num[] = {16, 18, 20, 22};  //TOFIX
const uint16_t i2s_tx_hs_num[] = {9, 11, 13, 36, 37, 38, 39};
const uint16_t i2s_rx_hs_num[] = {8, 10, 12, 14, 15, 16, 17};
const uint16_t spdif_tx_hs_num[] = {25, 27};
const uint16_t spdif_rx_hs_num[] = {24, 26};
const uint16_t tdm_rx_hs_num[] = {28, 29, 30, 31, 32, 33, 34, 35};
const uint16_t vad_rx_hs_num[] = {0, 1, 2, 3, 4, 5, 6, 7};

const csi_dma_ch_desc_t uart0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t iic0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t iic1_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t i2s0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t i2s1_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t i2s2_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t i2s3_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t spdif0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t spdif1_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t tdm_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t vad_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t spi3_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t spi2_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_spt_list_t dma_spt_list[] = {
    {DEV_DW_UART_TAG,   0, uart0_dma_ch_list},
    {DEV_DW_IIC_TAG,    0, iic0_dma_ch_list},
    {DEV_DW_IIC_TAG,    1, iic1_dma_ch_list},
    {DEV_WJ_I2S_TAG,    0, i2s0_dma_ch_list},
    {DEV_WJ_I2S_TAG,    1, i2s1_dma_ch_list},
    {DEV_WJ_I2S_TAG,    2, i2s2_dma_ch_list},
    {DEV_WJ_I2S_TAG,    3, i2s3_dma_ch_list},
    {DEV_WJ_SPDIF_TAG,  0, spdif0_dma_ch_list},
    {DEV_WJ_SPDIF_TAG,  1, spdif1_dma_ch_list},
    {DEV_WJ_TDM_TAG,    0, tdm_dma_ch_list},
    {DEV_WJ_VAD_TAG,    0, vad_dma_ch_list},
    {DEV_DW_SPI_TAG,    2, spi2_dma_ch_list}, /* spi2 */
    {0xFFFFU,         0xFFU,         NULL},
};
