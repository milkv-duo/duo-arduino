#ifndef _VARIANT_SOC_H_
#define _VARIANT_SOC_H
/* irq */
#define IRQ_LEVEL 0
#define IRQ_EDGE 3

#define NA 0xFF

#define TEMPSEN_IRQ_O NA
#define RTC_ALARM_O NA
#define RTC_PWR_BUTTON1_LONGPRESS_O NA
#define VBAT_DEB_IRQ_O NA
#define JPEG_INTERRUPT 16
#define H264_INTERRUPT 17
#define H265_INTERRUPT 18
#define VC_SBM_INT 19
#define ISP_INT 20
#define SC_INTR_0 21
#define VIP_INT_CSI_MAC0 22
#define VIP_INT_CSI_MAC1 23
#define LDC_INT 24
#define SDMA_INTR_CPU0 NA
#define SDMA_INTR_CPU1 NA
#define SDMA_INTR_CPU2 25
#define USB_IRQS NA
#define ETH0_SBD_INTR_O NA
#define ETH0_LPI_INTR_O NA
#define EMMC_WAKEUP_INTR NA
#define EMMC_INTR NA
#define SD0_WAKEUP_INTR NA
#define SD0_INTR NA
#define SD1_WAKEUP_INTR NA
#define SD1_INTR NA
#define SPI_NAND_INTR NA
#define I2S0_INT 26
#define I2S1_INT 27
#define I2S2_INT 28
#define I2S3_INT 29
#define UART0_INTR 30
#define UART1_INTR 31
#define UART2_INTR NA
#define UART3_INTR NA
#define UART4_INTR NA
#define I2C0_INTR 32
#define I2C1_INTR 33
#define I2C2_INTR 34
#define I2C3_INTR 35
#define I2C4_INTR 36
#define SPI_0_SSI_INTR 37
#define SPI_1_SSI_INTR 38
#define SPI_2_SSI_INTR NA
#define SPI_3_SSI_INTR NA
#define WDT0_INTR NA
#define WDT1_INTR NA
#define WDT2_INTR 39
#define KEYSCAN_IRQ 40
#define GPIO0_INTR_FLAG 41
#define GPIO1_INTR_FLAG 42
#define GPIO2_INTR_FLAG 43
#define GPIO3_INTR_FLAG 44
#define WGN0_IRQ 45
#define WGN1_IRQ NA
#define WGN2_IRQ NA
#define MBOX_INT1 46
#define IRRX_INT 47
#define GPIO_INT 48
#define UART_INT 49
#define SPI_INT NA
#define I2C_INT 50
#define WDT_INT 51
#define TPU_INTR NA
#define TDMA_INTERRUPT 52
#define SW_INT_0_CPU0 NA
#define SW_INT_1_CPU0 NA
#define SW_INT_0_CPU1 NA
#define SW_INT_1_CPU1 NA
#define SW_INT_0_CPU2 53
#define SW_INT_1_CPU2 54
#define TIMER_INTR_0 NA
#define TIMER_INTR_1 NA
#define TIMER_INTR_2 NA
#define TIMER_INTR_3 NA
#define TIMER_INTR_4 55
#define TIMER_INTR_5 56
#define TIMER_INTR_6 57
#define TIMER_INTR_7 58
#define PERI_FIREWALL_IRQ NA
#define HSPERI_FIREWALL_IRQ NA
#define DDR_FW_INTR NA
#define ROM_FIREWALL_IRQ NA
#define SPACC_IRQ 59
#define TRNG_IRQ NA
#define AXI_MON_INTR NA
#define DDRC_PI_PHY_INTR NA
#define SF_SPI_INT NA
#define EPHY_INT_N_O NA
#define IVE_INT 60
#define DBGSYS_APBUSMON_HANG_INT NA
#define INTR_SARADC NA
#define MBOX_INT_CA53 NA
#define MBOX_INT_C906 NA
#define MBOX_INT_C906_2ND 61
#define NPMUIRQ_0 NA
#define CTIIRQ_0 NA
#define NEXTERRIRQ NA

#define IRQF_TRIGGER_NONE 0x00000000
#define IRQF_TRIGGER_RISING 0x00000001
#define IRQF_TRIGGER_FALLING 0x00000002
#define IRQF_TRIGGER_HIGH 0x00000004
#define IRQF_TRIGGER_LOW 0x00000008
#define IRQF_TRIGGER_MASK                                                  \
	(IRQF_TRIGGER_HIGH | IRQF_TRIGGER_LOW | IRQF_TRIGGER_RISING |          \
	 IRQF_TRIGGER_FALLING)

#define NUM_IRQ (62)

#define CLINT_MTIME(cnt)             asm volatile("csrr %0, time\n" : "=r"(cnt) :: "memory");

/* PLIC */
#define PLIC_PRIORITY0          (PLIC_BASE + 0x0)
#define PLIC_PRIORITY1          (PLIC_BASE + 0x4)
#define PLIC_PRIORITY2          (PLIC_BASE + 0x8)
#define PLIC_PRIORITY3          (PLIC_BASE + 0xc)
#define PLIC_PRIORITY4          (PLIC_BASE + 0x10)

#define PLIC_PENDING1           (PLIC_BASE + 0x1000)
#define PLIC_PENDING2           (PLIC_BASE + 0x1004)
#define PLIC_PENDING3           (PLIC_BASE + 0x1008)
#define PLIC_PENDING4           (PLIC_BASE + 0x100C)

#define PLIC_ENABLE1            (PLIC_BASE + 0x2000)
#define PLIC_ENABLE2            (PLIC_BASE + 0x2004)
#define PLIC_ENABLE3            (PLIC_BASE + 0x2008)
#define PLIC_ENABLE4            (PLIC_BASE + 0x200C)

#define PLIC_THRESHOLD          (PLIC_BASE + 0x200000)
#define PLIC_CLAIM              (PLIC_BASE + 0x200004)

#define DW_TIMER0_BASE              0x030A0000UL
#define DW_TIMER0_SIZE              0x14U

#define DW_TIMER1_BASE              (DW_TIMER0_BASE+DW_TIMER0_SIZE)
#define DW_TIMER1_SIZE              DW_TIMER0_SIZE

#define DW_TIMER2_BASE              (DW_TIMER1_BASE+DW_TIMER1_SIZE)
#define DW_TIMER2_SIZE              DW_TIMER1_SIZE

#define DW_TIMER3_BASE              (DW_TIMER2_BASE+DW_TIMER2_SIZE)
#define DW_TIMER3_SIZE              DW_TIMER2_SIZE

#define DW_TIMER4_BASE              (DW_TIMER3_BASE+DW_TIMER3_SIZE)
#define DW_TIMER4_SIZE              DW_TIMER3_SIZE

#define DW_TIMER5_BASE              (DW_TIMER4_BASE+DW_TIMER4_SIZE)
#define DW_TIMER5_SIZE              DW_TIMER4_SIZE

#define DW_TIMER6_BASE              (DW_TIMER5_BASE+DW_TIMER5_SIZE)
#define DW_TIMER6_SIZE              DW_TIMER5_SIZE

#define DW_TIMER7_BASE              (DW_TIMER6_BASE+DW_TIMER6_SIZE)
#define DW_TIMER7_SIZE              DW_TIMER6_SIZE

/* RISC-V */
#define CLINT_BASE              0x74000000
#define PLIC_BASE               0x70000000

/* CLINT */
#define CLINT_TIMECMPL0         (CLINT_BASE + 0x4000)
#define CLINT_TIMECMPH0         (CLINT_BASE + 0x4004)

#define CONFIG_GPIO_NUM             5
#define CONFIG_IRQ_NUM              1023U
#define CONFIG_REAL_IRQ_CNT         105

#define PIN_MUX_BASE            0x03001000UL
#define PIN_CFG_BASE            0x03001000UL
#define PWR_PIN_CFG_BASE        0x05027000UL

#define DW_GPIO0_BASE               0x03020000UL
#define DW_GPIO1_BASE               0x03021000UL
#define DW_GPIO2_BASE               0x03022000UL
#define DW_GPIO3_BASE               0x03023000UL
#define DW_RTC_GPIO_BASE            0x05021000UL

#define CVI_WDT0_BASE               0x03010000
#define CVI_WDT1_BASE               0x03011000
#define CVI_WDT2_BASE               0x03012000
#define CVI_RTC_WDT_BASE            0x0502D000
#define CVI_WDT_SIZE                0x1000

#define CVI_TEMPSEN_BASE            0x030E0000
#define CVI_TEMPSEN_SIZE            0x10000

#define DW_UART0_BASE               0x04140000UL
#define DW_UART0_SIZE               0x1000U

#define DW_UART1_BASE               0x04150000UL
#define DW_UART1_SIZE               0x1000U

#define DW_UART2_BASE               0x04160000UL
#define DW_UART2_SIZE               0x1000U

#define DW_UART3_BASE               0x04170000UL
#define DW_UART3_SIZE               0x1000U

#define DW_UART4_BASE               0x041C0000UL
#define DW_UART4_SIZE               0x1000U

#define DW_IIC0_BASE                0x04000000UL
#define DW_IIC0_SIZE                0x1000U

#define DW_IIC1_BASE                0x04010000UL
#define DW_IIC1_SIZE                0x1000U

#define DW_IIC2_BASE                0x04020000UL
#define DW_IIC2_SIZE                0x1000U

#define DW_IIC3_BASE                0x04030000UL
#define DW_IIC3_SIZE                0x1000U

#define DW_IIC4_BASE                0x04040000UL
#define DW_IIC4_SIZE                0x1000U

#define DW_MAC_BASE                 0x04070000UL
#define DW_MAC_SIZE                 0x10000U

#define ADC_BASE                    0x030F0000
#define RTC_ADC_BASE                0x0502C000
#define ADC_SIZE                    0x1000

#define CVI_PWM0_BASE               0x03060000
#define CVI_PWM1_BASE               0x03061000
#define CVI_PWM2_BASE               0x03062000
#define CVI_PWM3_BASE               0x03063000

#define DW_SPI_REG_SIZE             (0x10000UL)
#define DW_SPI0_BASE                (0x04180000UL)
#define DW_SPI1_BASE                (DW_SPI0_BASE + 1 * DW_SPI_REG_SIZE)
#define DW_SPI2_BASE                (DW_SPI0_BASE + 2 * DW_SPI_REG_SIZE)
#define DW_SPI3_BASE                (DW_SPI0_BASE + 3 * DW_SPI_REG_SIZE)

#define PLIC_BASE           (0x70000000UL)
#define CORET_BASE          (PLIC_BASE + 0x4000000UL)               /*!< CORET Base Address */
#define PLIC                ((PLIC_Type *)PLIC_BASE)

#define DW_SDIO0_BASE               0x04320000  // SDIO
#define DW_SDIO1_BASE               0x04310000  // SD
#define DW_SDIO2_BASE               0x04300000  // MMC

#endif
