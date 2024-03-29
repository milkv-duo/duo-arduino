/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "variant.h"
#include "csi_pin.h"

const pin_reg_offset_t csi_pin_reg_offset[] = {
    {SD0_CLK, 0XA00, 0X000},
    {SD0_CMD, 0XA04, 0X004},
    {SD0_D0, 0XA08, 0X008},
    {SD0_D1, 0XA0c, 0X00c},
    {SD0_D2, 0XA10, 0X010},
    {SD0_D3, 0XA14, 0X014},
    {SD0_CD, 0X900, 0X018},
    {SD0_PWR_EN, 0X904, 0X01c},
    {SPK_EN, 0X908, 0X020},
    {UART0_TX, 0X90c, 0X024},
    {UART0_RX, 0X910, 0X028},
    {SPINOR_HOLD_X, 0X914, 0X02c},
    {SPINOR_SCK, 0X918, 0X030},
    {SPINOR_MOSI, 0X91c, 0X034},
    {SPINOR_WP_X, 0X920, 0X038},
    {SPINOR_MISO, 0X924, 0X03c},
    {SPINOR_CS_X, 0X928, 0X040},
    {JTAG_CPU_TMS, 0X92c, 0X044},
    {JTAG_CPU_TCK, 0X930, 0X048},
    {IIC0_SCL, 0X934, 0X04c},
    {IIC0_SDA, 0X938, 0X050},
    {AUX0, 0X93c, 0X054},
    {GPIO_ZQ, 0X000, 0X058},
    {PWR_VBAT_DET, 0X004, 0X05c},
    {PWR_RSTN, 0X008, 0X060},
    {PWR_SEQ1, 0X00c, 0X064},
    {PWR_SEQ2, 0X010, 0X068},
    {PWR_WAKEUP0, 0X018, 0X06c},
    {PWR_BUTTON1, 0X01c, 0X070},
    {XTAL_XIN, 0X020, 0X074},
    {PWR_GPIO0, 0X024, 0X078},
    {PWR_GPIO1, 0X028, 0X07c},
    {PWR_GPIO2, 0X02c, 0X080},
    {SD1_GPIO1, 0X030, 0X084},
    {SD1_GPIO0, 0X034, 0X088},
    {SD1_D3, 0X038, 0X08c},
    {SD1_D2, 0X03c, 0X090},
    {SD1_D1, 0X040, 0X094},
    {SD1_D0, 0X044, 0X098},
    {SD1_CMD, 0X048, 0X09c},
    {SD1_CLK, 0X04c, 0X0a0},
    {PWM0_BUCK, 0X800, 0X0a4},
    {ADC1, 0X804, 0X0a8},
    {USB_VBUS_DET, 0X808, 0X0ac},
    {PAD_ETH_TXP, 0XFFFF, 0X0c0},
    {PAD_ETH_TXM, 0XFFFF, 0X0c4},
    {PAD_ETH_RXP, 0XFFFF, 0X0c8},
    {PAD_ETH_RXM, 0XFFFF, 0X0cc},
    {GPIO_RTX, 0XC00, 0X0d0},
    {PAD_MIPIRX4N, 0XC04, 0X0d4},
    {PAD_MIPIRX4P, 0XC08, 0X0d8},
    {PAD_MIPIRX3N, 0XC0c, 0X0dc},
    {PAD_MIPIRX3P, 0XC10, 0X0e0},
    {PAD_MIPIRX2N, 0XC14, 0X0e4},
    {PAD_MIPIRX2P, 0XC18, 0X0e8},
    {PAD_MIPIRX1N, 0XC1c, 0X0ec},
    {PAD_MIPIRX1P, 0XC20, 0X0f0},
    {PAD_MIPIRX0N, 0XC24, 0X0f4},
    {PAD_MIPIRX0P, 0XC28, 0X0f8},
    {PAD_MIPI_TXM2, 0XC2c, 0X0fc},
    {PAD_MIPI_TXP2, 0XC30, 0X100},
    {PAD_MIPI_TXM1, 0XC34, 0X104},
    {PAD_MIPI_TXP1, 0XC38, 0X108},
    {PAD_MIPI_TXM0, 0XC3c, 0X10c},
    {PAD_MIPI_TXP0, 0XC40, 0X110},
    {PAD_AUD_AINL_MIC, 0XFFFF, 0X120},
    {PAD_AUD_AOUTR, 0XFFFF, 0X12c},
    {MUX_SPI1_MISO, 0XFFFF, 0X0b0},
    {MUX_SPI1_MOSI, 0XFFFF, 0X0b4},
    {MUX_SPI1_CS, 0XFFFF, 0X0b8},
    {MUX_SPI1_SCK, 0XFFFF, 0X0bc},
    {0xffffffffU, 0XFFFF, 0XFFFF},
};

const csi_pinmap_t csi_gpio_pinmap[] = {
    {SD0_CLK, 0, 7, PIN_FUNC_GPIO},
    {SD0_CMD, 0, 8, PIN_FUNC_GPIO},
    {SD0_D0, 0, 9, PIN_FUNC_GPIO},
    {SD0_D1, 0, 10, PIN_FUNC_GPIO},
    {SD0_D2, 0, 11, PIN_FUNC_GPIO},
    {SD0_D3, 0, 12, PIN_FUNC_GPIO},
    {SD0_CD, 0, 13, PIN_FUNC_GPIO},
    {SD0_PWR_EN, 0, 14, PIN_FUNC_GPIO},
    {SPK_EN, 0, 15, PIN_FUNC_GPIO},
    {UART0_TX, 0, 16, PIN_FUNC_GPIO},
    {UART0_RX, 0, 17, PIN_FUNC_GPIO},
    {SPINOR_HOLD_X, 0, 26, PIN_FUNC_GPIO},
    {SPINOR_SCK, 0, 22, PIN_FUNC_GPIO},
    {SPINOR_MOSI, 0, 25, PIN_FUNC_GPIO},
    {SPINOR_WP_X, 0, 27, PIN_FUNC_GPIO},
    {SPINOR_MISO, 0, 23, PIN_FUNC_GPIO},
    {SPINOR_CS_X, 0, 24, PIN_FUNC_GPIO},
    {JTAG_CPU_TMS, 0, 19, PIN_FUNC_GPIO},
    {JTAG_CPU_TCK, 0, 18, PIN_FUNC_GPIO},
    {IIC0_SCL, 0, 28, PIN_FUNC_GPIO},
    {IIC0_SDA, 0, 29, PIN_FUNC_GPIO},
    {AUX0, 0, 30, PIN_FUNC_GPIO},
    {PWM0_BUCK, 1, 0, PIN_FUNC_GPIO},
    {ADC1, 1, 3, PIN_FUNC_GPIO},
    {USB_VBUS_DET, 1, 6, PIN_FUNC_GPIO},
    {PAD_ETH_TXP, 1, 25, PIN_FUNC_GPIO},
    {PAD_ETH_TXM, 1, 24, PIN_FUNC_GPIO},
    {PAD_ETH_RXP, 1, 27, PIN_FUNC_GPIO},
    {PAD_ETH_RXM, 1, 26, PIN_FUNC_GPIO},
    {GPIO_RTX, 1, 23, PIN_FUNC_GPIO},
    {PAD_MIPIRX4N, 2, 2, PIN_FUNC_GPIO},
    {PAD_MIPIRX4P, 2, 3, PIN_FUNC_GPIO},
    {PAD_MIPIRX3N, 2, 4, PIN_FUNC_GPIO},
    {PAD_MIPIRX3P, 2, 5, PIN_FUNC_GPIO},
    {PAD_MIPIRX2N, 2, 6, PIN_FUNC_GPIO},
    {PAD_MIPIRX2P, 2, 7, PIN_FUNC_GPIO},
    {PAD_MIPIRX1N, 2, 8, PIN_FUNC_GPIO},
    {PAD_MIPIRX1P, 2, 9, PIN_FUNC_GPIO},
    {PAD_MIPIRX0N, 2, 10, PIN_FUNC_GPIO},
    {PAD_MIPIRX0P, 2, 11, PIN_FUNC_GPIO},
    {PAD_MIPI_TXM2, 2, 16, PIN_FUNC_GPIO},
    {PAD_MIPI_TXP2, 2, 17, PIN_FUNC_GPIO},
    {PAD_MIPI_TXM1, 2, 14, PIN_FUNC_GPIO},
    {PAD_MIPI_TXP1, 2, 15, PIN_FUNC_GPIO},
    {PAD_MIPI_TXM0, 2, 12, PIN_FUNC_GPIO},
    {PAD_MIPI_TXP0, 2, 13, PIN_FUNC_GPIO},
    {PAD_AUD_AINL_MIC, 2, 23, PIN_FUNC_GPIO},
    {PAD_AUD_AOUTR, 2, 24, PIN_FUNC_GPIO},
    {SD1_GPIO1, 4, 26, PIN_FUNC_GPIO},
    {SD1_GPIO0, 4, 25, PIN_FUNC_GPIO},
    {SD1_D2, 4, 19, PIN_FUNC_GPIO},
    {SD1_D1, 4, 20, PIN_FUNC_GPIO},
    {SD1_CLK, 4, 23, PIN_FUNC_GPIO},
    {SD1_CMD, 4, 22, PIN_FUNC_GPIO},
    {SD1_D0, 4, 21, PIN_FUNC_GPIO},
    {SD1_D3, 4, 18, PIN_FUNC_GPIO},
    {PWR_SEQ2, 4, 4, PIN_FUNC_GPIO},
    {MUX_SPI1_MISO, 1, 8, PIN_FUNC_GPIO},
    {MUX_SPI1_MOSI, 1, 7, PIN_FUNC_GPIO},
    {MUX_SPI1_CS, 1, 10, PIN_FUNC_GPIO},
    {MUX_SPI1_SCK, 1, 9, PIN_FUNC_GPIO},
};

dev_pin_map_t gpio_map[] = {
    {SD0_CLK, SD0_CLK__XGPIOA_7, 0},
    {SD0_CMD, SD0_CMD__XGPIOA_8, 0},
    {SD0_D0, SD0_D0__XGPIOA_9, 0},
    {SD0_D1, SD0_D1__XGPIOA_10, 0},
    {SD0_D2, SD0_D2__XGPIOA_11, 0},
    {SD0_D3, SD0_D3__XGPIOA_12, 0},
    {SD0_CD, SD0_CD__XGPIOA_13, 0},
    {SD0_PWR_EN, SD0_PWR_EN__XGPIOA_14, 0},
    {SPK_EN, SPK_EN__XGPIOA_15, 0},
    {UART0_TX, UART0_TX__XGPIOA_16, 0},
    {UART0_RX, UART0_RX__XGPIOA_17, 0},
    {0xFE, JTAG_CPU_TCK, JTAG_CPU_TCK__XGPIOA_18, 0},
    {0xFE, JTAG_CPU_TMS, JTAG_CPU_TMS__XGPIOA_19, 0},
    {SPINOR_SCK, SPINOR_SCK__XGPIOA_22, 0},
    {SPINOR_MISO, SPINOR_MISO__XGPIOA_23, 0},
    {SPINOR_CS_X, SPINOR_CS_X__XGPIOA_24, 0},
    {SPINOR_MOSI, SPINOR_MOSI__XGPIOA_25, 0},
    {SPINOR_HOLD_X, SPINOR_HOLD_X__XGPIOA_26, 0},
    {SPINOR_WP_X, SPINOR_WP_X__XGPIOA_27, 0},
    {IIC0_SCL, IIC0_SCL__XGPIOA_28, 0},
    {IIC0_SDA, IIC0_SDA__XGPIOA_29, 0},
    {AUX0, AUX0__XGPIOA_30, 0},
    {ADC1, ADC1__XGPIOB_3, 1},
    {USB_VBUS_DET, USB_VBUS_DET__XGPIOB_6, 1},
    {PAD_ETH_TXM, PAD_ETH_TXM__XGPIOB_24, 1},
    {PAD_ETH_TXP, PAD_ETH_TXP__XGPIOB_25, 1},
    {PAD_ETH_RXM, PAD_ETH_RXM__XGPIOB_26, 1},
    {PAD_ETH_RXP, PAD_ETH_RXP__XGPIOB_27, 1},
    {PAD_MIPIRX4N, PAD_MIPIRX4N__XGPIOC_2, 2},
    {PAD_MIPIRX4P, PAD_MIPIRX4P__XGPIOC_3, 2},
    {PAD_MIPIRX3N, PAD_MIPIRX3N__XGPIOC_4, 2},
    {PAD_MIPIRX3P, PAD_MIPIRX3P__XGPIOC_5, 2},
    {PAD_MIPIRX2N, PAD_MIPIRX2N__XGPIOC_6, 2},
    {PAD_MIPIRX2P, PAD_MIPIRX2P__XGPIOC_7, 2},
    {PAD_MIPIRX1N, PAD_MIPIRX1N__XGPIOC_8, 2},
    {PAD_MIPIRX1P, PAD_MIPIRX1P__XGPIOC_9, 2},
    {PAD_MIPIRX0N, PAD_MIPIRX0N__XGPIOC_10, 2},
    {PAD_MIPIRX0P, PAD_MIPIRX0P__XGPIOC_11, 2},
    {PAD_AUD_AINL_MIC, PAD_AUD_AINL_MIC__XGPIOC_23, 2},
    {PAD_AUD_AOUTR, PAD_AUD_AOUTR__XGPIOC_24, 2},
    {SD1_GPIO1, SD1_GPIO1__PWR_GPIO_26, 4},
    {SD1_GPIO0, SD1_GPIO0__PWR_GPIO_25, 4},
    {SD1_D2, SD1_D2__PWR_GPIO_19, 4},
    {SD1_D1, SD1_D1__PWR_GPIO_20, 4},
    {SD1_CLK, SD1_CLK__PWR_GPIO_23, 4},
    {SD1_CMD, SD1_CMD__PWR_GPIO_22, 4},
    {SD1_D0, SD1_D0__PWR_GPIO_21, 4},
    {SD1_D3, SD1_D3__PWR_GPIO_18, 4},
    {PWR_SEQ2, PWR_SEQ2__PWR_GPIO_4, 4},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t pwm_map[] = {
    {ADC1, ADC1__PWM_3, 3},
    {USB_VBUS_DET, USB_VBUS_DET__PWM_4, 4},
    {SD1_GPIO1, SD1_GPIO1__PWM_10, 10},
    {SD1_GPIO0, SD1_GPIO0__PWM_11, 11},
    {UART0_TX, UART0_TX__PWM_4, 4},
    {SD1_D3, SD1_D3__PWM_4, 4},
    {UART0_RX, UART0_RX__PWM_5, 5},
    {SD1_D2, SD1_D2__PWM_5, 5},
    {SD1_D1, SD1_D1__PWM_6, 6},
    {SD1_D0, SD1_D0__PWM_7, 7},
    {SD1_CMD, SD1_CMD__PWM_8, 8},
    {SD1_CLK, SD1_CLK__PWM_9, 9},
    {SD0_D3, SD0_D3__PWM_10, 10},
    {SD0_D2, SD0_D2__PWM_11, 11},
    {PAD_ETH_TXM, PAD_ETH_TXM__PWM_12, 12},
    {SD0_D1, SD0_D1__PWM_12, 12},
    {PAD_ETH_TXP, PAD_ETH_TXP__PWM_13, 13},
    {SD0_D0, SD0_D0__PWM_13, 13},
    {PAD_ETH_RXM, PAD_ETH_RXM__PWM_14, 14},
    {SD0_CMD, SD0_CMD__PWM_14, 14},
    {PAD_ETH_TXP, PAD_ETH_RXP__PWM_15, 15},
    {SD0_CLK, SD0_CLK__PWM_15, 15},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t adc_map[] = {
    {ADC1, ADC1__XGPIOB_3, 0},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t uart_rx_map[] = {
    {UART0_RX, UART0_RX__UART0_RX, 0},
    {SD0_D2, SD0_D2__UART1_RX, 1},
    {UART0_RX, UART0_RX__UART1_RX, 1},
    {IIC0_SDA, IIC0_SDA__UART1_RX, 1},
    {IIC0_SDA, IIC0_SDA__UART2_RX, 2},
    {SD1_D1, SD1_D1__UART2_RX, 2},
    {PAD_ETH_TXP, PAD_ETH_TXP__UART3_RX, 3},
    {SD1_D1, SD1_D1__UART3_RX, 3},
    {SD0_D3, SD0_D3__UART3_RX, 3},
    {SD1_GPIO0, SD1_GPIO0__UART4_RX, 4},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t uart_tx_map[] = {
    {UART0_TX, UART0_TX__UART0_TX, 0},
    {SD0_D1, SD0_D1__UART1_TX, 1},
    {UART0_TX, UART0_TX__UART1_TX, 1},
    {IIC0_SCL, IIC0_SCL__UART1_TX, 1},
    {IIC0_SCL, IIC0_SCL__UART2_TX, 2},
    {SD1_D2, SD1_D2__UART2_TX, 2},
    {PAD_ETH_RXP, PAD_ETH_RXP__UART3_TX, 3},
    {SD1_D2, SD1_D2__UART3_TX, 3},
    {SD0_D0, SD0_D0__UART3_TX, 3},
    {SD1_GPIO1, SD1_GPIO1__UART4_TX, 4},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t iic_sda_map[] = {
    {IIC0_SDA, IIC0_SDA__IIC0_SDA, 0},
    {PAD_MIPIRX4P, PAD_MIPIRX4P__IIC0_SDA, 0},
    {PAD_MIPIRX4N, PAD_MIPIRX4N__IIC1_SDA, 0},
    {PAD_MIPIRX1P, PAD_MIPIRX1P__IIC1_SDA, 1},
    {SD1_D0, SD1_D0__IIC1_SDA, 1},
    {SD0_CLK, SD0_CLK__IIC1_SDA, 1}, // TODO 2
    {SD0_D1, SD0_D1__IIC1_SDA, 1},
    {SD1_D1, SD1_D1__IIC1_SDA, 1},
    {SD1_CLK, SD1_CLK__IIC3_SDA, 3},
    {PAD_MIPIRX2P, PAD_MIPIRX2P__IIC4_SDA, 4},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t iic_scl_map[] = {
    {IIC0_SCL, IIC0_SCL__IIC0_SCL, 0},
    {PAD_MIPIRX4N, PAD_MIPIRX4N__IIC0_SCL, 0},
    {PAD_MIPIRX4P, PAD_MIPIRX4P__IIC1_SCL, 1},
    {PAD_MIPIRX0N, PAD_MIPIRX0N__IIC1_SCL, 1},
    {SD1_D3, SD1_D3__IIC1_SCL, 1},
    {SD0_CMD, SD0_CMD__IIC1_SCL, 1}, // TODO 2
    {SD0_D2, SD0_D2__IIC1_SCL, 1},
    {SD1_D2, SD1_D2__IIC1_SCL, 1},
    {SD1_CMD, SD1_CMD__IIC3_SCL, 3},
    {PAD_MIPIRX2N, PAD_MIPIRX2N__IIC4_SCL, 4},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t spi_cs_map[] = {
    {SD0_D3, SD0_D3__SPI0_CS_X, 0},
    {PAD_ETH_RXM, PAD_ETH_RXM__SPI1_CS_X, 1},
    {SD1_D3, SD1_D3__SPI2_CS_X, 2},
    {0xFFFFFFFF, 0xFF, 0}};

dev_pin_map_t spi_sck_map[] = {
    {SD0_CLK, SD0_CLK__SPI0_SCK, 2, 0},
    {PAD_ETH_RXP, PAD_ETH_RXP__SPI1_SCK, 1},
    {SD1_CLK, SD1_CLK__SPI2_SCK, 2},
    {0xFF, 0xFFFFFFFF, 0}};

dev_pin_map_t spi_miso_map[] = {
    {SD0_D0, SD0_D0__SPI0_SDI, 2, 0},
    {PAD_ETH_TXM, PAD_ETH_TXM__SPI1_SDI, 1},
    {SD1_D0, SD1_D0__SPI2_SDI, 2},
    {0xFF, 0xFFFFFFFF, 0}};

dev_pin_map_t spi_mosi_map[] = {
    {SD0_CMD, SD0_CMD__SPI0_SDO, 2, 0},
    {PAD_ETH_TXP, PAD_ETH_TXP__SPI1_SDO, 1},
    {SD1_CMD, SD1_CMD__SPI2_SDO, 2},
    {0xFF, 0xFFFFFFFF, 0}};

pin_name_t out_pin_map[] = {
    PAD_AUD_AOUTR,
    IIC0_SCL,
    IIC0_SDA,
    0xFF,
    SD1_GPIO1,
    SD1_GPIO0,
    SD1_D2,
    SD1_D1,
    0xFF,
    SD1_CLK,
    SD1_CMD,
    SD1_D0,
    SD1_D3,
    0xFF,
    PAD_MIPIRX1P,
    PAD_MIPIRX0N,
    UART0_TX,
    UART0_RX,
    0xFF,
    SD0_PWR_EN,
    SPK_EN,
    SPINOR_MISO,
    SPINOR_CS_X,
    0xFF,
    SPINOR_SCK,
    SPINOR_MOSI,
    SPINOR_WP_X,
    SPINOR_HOLD_X,
    0xFF,
    PWR_SEQ2,
    0xFF,
    ADC1,
    USB_VBUS_DET,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF
};

pin_func_t out_pin_func_map[] = {
    PAD_AUD_AOUTR__XGPIOC_24,
    IIC0_SCL__XGPIOA_28,
    IIC0_SDA__XGPIOA_29,
    0xFF,
    SD1_GPIO1__PWR_GPIO_26,
    SD1_GPIO0__PWR_GPIO_25,
    SD1_D2__PWR_GPIO_19,
    SD1_D1__PWR_GPIO_20,
    0xFF,
    SD1_CLK__PWR_GPIO_23,
    SD1_CMD__PWR_GPIO_22,
    SD1_D0__PWR_GPIO_21,
    SD1_D3__PWR_GPIO_18,
    0xFF,
    PAD_MIPIRX1P__XGPIOC_9,
    PAD_MIPIRX0N__XGPIOC_10,
    UART0_TX__XGPIOA_16,
    UART0_RX__XGPIOA_17,
    0xFF,
    SD0_PWR_EN__XGPIOA_14,
    SPK_EN__XGPIOA_15,
    SPINOR_MISO__XGPIOA_23,
    SPINOR_CS_X__XGPIOA_24,
    0xFF,
    SPINOR_SCK__XGPIOA_22,
    SPINOR_MOSI__XGPIOA_25,
    SPINOR_WP_X__XGPIOA_27,
    SPINOR_HOLD_X__XGPIOA_26,
    0xFF,
    PWR_SEQ2__PWR_GPIO_4,
    0xFF,
    ADC1__XGPIOB_3,
    USB_VBUS_DET__XGPIOB_6,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF
};
