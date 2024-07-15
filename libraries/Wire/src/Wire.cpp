/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

extern "C" {
#include <string.h>
}

#include "Arduino.h"
#include "Wire.h"
#include "dw_iic_ll.h"
#include "csi_pin.h"

extern dev_pin_map_t iic_sda_map[];
extern dev_pin_map_t iic_scl_map[];

static int i2c_slave_irq(int irq, void *TwoWireInst);

TwoWire::TwoWire(uint8_t idx, uint8_t pinSDA, uint8_t pinSCL) :
_uc_pinSDA(pinSDA),
_uc_pinSCL(pinSCL),
_idx(idx),
_timeout(-1),
_master(true),
transmissionBegun(false)
{
}

void TwoWire::begin(csi_iic_addr_mode_t addr_mode)
{
    //Master Mode
    const dev_pin_map_t* sda_pin = target_pin_number_to_dev(_uc_pinSDA, iic_sda_map, _idx);
    const dev_pin_map_t* scl_pin = target_pin_number_to_dev(_uc_pinSCL, iic_scl_map, _idx);

    if (sda_pin == NULL || scl_pin == NULL) {
        pr_err("pin GPIO %d or %d are not used as IIC func\n", _uc_pinSDA, _uc_pinSDA);
        return;
    }

    if (csi_pin_set_mux(sda_pin->name, sda_pin->func)) {
        pr_err("pin GPIO %d fails to config as iic_sda func\n", _uc_pinSDA);
        return;
    };

    if (csi_pin_set_mux(scl_pin->name, scl_pin->func)) {
        pr_err("pin GPIO %d fails to config as iic_scl func\n", _uc_pinSDA);
        return;
    };

    if (csi_iic_init(&_iic, _idx) != CSI_OK) {
        pr_err("iic %d init fails", _idx);
        return;
    }

    _master = true;
    csi_iic_mode(&_iic, IIC_MODE_MASTER);
    csi_iic_addr_mode(&_iic, addr_mode);
}

void TwoWire::begin(uint16_t address, csi_iic_addr_mode_t addr_mode)
{
    //Slave mode
    const dev_pin_map_t* sda_pin = target_pin_number_to_dev(_uc_pinSDA, iic_sda_map, _idx);
    const dev_pin_map_t* scl_pin = target_pin_number_to_dev(_uc_pinSCL, iic_scl_map, _idx);

    if (sda_pin == NULL || scl_pin == NULL) {
        pr_err("pin GPIO %d or %d are not used as IIC func\n", _uc_pinSDA, _uc_pinSCL);
        return;
    }

    if (csi_pin_set_mux(sda_pin->name, sda_pin->func)) {
        pr_err("pin GPIO %d fails to config as iic_sda func\n", _uc_pinSDA);
        return;
    };

    if (csi_pin_set_mux(scl_pin->name, scl_pin->func)) {
        pr_err("pin GPIO %d fails to config as iic_scl func\n", _uc_pinSCL);
        return;
    };

    csi_iic_t *iic = &_iic;
    if (csi_iic_init(iic, _idx) != CSI_OK) {
        pr_err("iic %d init fails", _idx);
        return;
    }

    _master = false;

    csi_iic_mode(iic, IIC_MODE_SLAVE);
    csi_iic_addr_mode(iic, addr_mode);
    this->_addr = address;
    csi_iic_own_addr(iic, address);

    dw_iic_regs_t *iic_base = (dw_iic_regs_t *)HANDLE_REG_BASE(iic);
    attach_irq((uint32_t)iic->dev.irq_num, &i2c_slave_irq, 0, "iic slave irq", this);
    enable_irq((uint32_t)iic->dev.irq_num);
    dw_iic_disable(iic_base);
    dw_iic_clear_all_irq(iic_base);
    dw_iic_disable_all_irq(iic_base);
    dw_iic_slave_enable_irq(iic_base);
    iic_base->IC_SLV_DAT_NACK_ONLY = 0;
    dw_iic_enable(iic_base);
}

void TwoWire::setClock(uint32_t baudrate)
{
    csi_iic_speed_t speed = IIC_BUS_SPEED_HIGH;
    if (baudrate <= 1000000) {
        speed = IIC_BUS_SPEED_FAST_PLUS;
    } else if (baudrate <= 400000) {
        speed = IIC_BUS_SPEED_FAST;
    } else if (baudrate <= 100000) {
        speed = IIC_BUS_SPEED_STANDARD;
    }
    csi_iic_speed(&_iic, speed);
}

void TwoWire::end()
{
    csi_iic_uninit(&_iic);
}

void TwoWire::print()
{
    csi_iic_t *iic = &_iic;
    dw_iic_regs_t *iic_base;
    iic_base = (dw_iic_regs_t *)HANDLE_REG_BASE(iic);
    pr_printf("Wire%d: %d\n\r", _idx, dw_iic_get_iic_status(iic_base));
    iic_dump_register(iic_base);
}

size_t TwoWire::requestFrom(uint16_t address, size_t quantity, bool stopBit)
{
    if (!_master) {
        pr_err("cannot call requestFrom from slave device\r\n");
    }
    uint8_t data[256];
    
    if (quantity == 0) {
        return 0;
    } else if (quantity > 256) {
        quantity = 256;
    }

    rxBuffer.clear();
    int ret = csi_iic_master_receive(&_iic, address, data, quantity, _timeout, stopBit);

    if (ret > 0) {
        for (int i = 0; i < ret; i++) {
            if (!rxBuffer.availableForStore())
                break;
            rxBuffer.store_char(data[i]);
        }
    }
    return ret;
}

size_t TwoWire::requestFrom(uint16_t address, size_t quantity)
{
    return requestFrom(address, quantity, true);
}

void TwoWire::beginTransmission(uint16_t address)
{
    if (!_master) {
        pr_err("cannot call beginTransmission from slave device\r\n");
    }
    // save address of target and clear buffer
    txAddress = address;
    txBuffer.clear();

    transmissionBegun = true;
}

// Errors:
//  0 : Success
//  1 : Data too long
//  2 : NACK on transmit of address
//  3 : NACK on transmit of data
//  4 : Other error
uint8_t TwoWire::endTransmission(bool stopBit)
{
    if (!_master) {
        pr_err("cannot call endTransmission from slave device\r\n");
    }

    uint8_t data[256];
    int i = 0;

    transmissionBegun = false;
    while (txBuffer.available()) {
        data[i++] = txBuffer.read_char();
    }

    int ret = csi_iic_master_send(&_iic, txAddress, data, i, _timeout, stopBit);
    switch (ret) {
    case CSI_OK:
        return 0;
    case CSI_TIMEOUT:
        return 5;
    default:
	if (ret > 0)
            return 0;
        else
	    return 4;
    }

    return 0;
}

uint8_t TwoWire::endTransmission()
{
    return endTransmission(true);
}

size_t TwoWire::write(uint8_t ucData)
{
    // No writing, without begun transmission or a full buffer
    if ((_master && !transmissionBegun) || txBuffer.isFull()) {
        return 0 ;
    }

    txBuffer.store_char( ucData ) ;

    return 1 ;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    //Try to store all data
    for (size_t i = 0; i < quantity; ++i) {
        //Return the number of data stored, when the buffer is full (if write return 0)
        if (!write(data[i]))
            return i;
    }

    //All data stored
    return quantity;
}

int TwoWire::available(void)
{
    return rxBuffer.available();
}

int TwoWire::read(void)
{
    return rxBuffer.read_char();
}

int TwoWire::peek(void)
{
    return rxBuffer.peek();
}

void TwoWire::flush(void)
{
    txBuffer.clear();
    rxBuffer.clear();
}

void TwoWire::onReceive(void(*function)(int))
{
    if (_master) {
        pr_err("cannot call onReceive from master device\r\n");
    }
    onReceiveCallback = function;
}

void TwoWire::onRequest(void(*function)(void))
{
    if (_master) {
        pr_err("cannot call onRequest from master device\r\n");
    }
    onRequestCallback = function;
}

void TwoWire::onService(void)
{
    if (_master) {
        pr_err("cannot call onService from master device\r\n");
    }

    csi_iic_t *iic = &_iic;
    dw_iic_regs_t *iic_base;
    iic_base = (dw_iic_regs_t *)HANDLE_REG_BASE(iic);

    uint32_t intr_state;
    intr_state = dw_iic_read_clear_intrbits(iic_base);

    uint32_t count, iic_rx_fifo;

    uint32_t emptyfifo;
    uint32_t transmit_fifo_Level;
    uint32_t i;

    if (intr_state & DW_IIC_INTR_RD_REQ) {
        if (onRequestCallback)
            onRequestCallback();
        transmit_fifo_Level = dw_iic_get_transmit_fifo_num(iic_base);
        emptyfifo = (txBuffer.available() > (uint32_t)(IIC_MAX_FIFO - transmit_fifo_Level)) ? 
                    (uint32_t)(IIC_MAX_FIFO - transmit_fifo_Level) : iic->size;

        for (i = 0U; i < emptyfifo; i++) {
            dw_iic_transmit_data(iic_base, txBuffer.read_char());
        }
    }

    /* fifo buff is full read from fifo and send read cmd */
    if (intr_state & DW_IIC_INTR_RX_FULL) {
        iic_rx_fifo = dw_iic_get_receive_fifo_num(iic_base);
    
        for (count = 0U; count < iic_rx_fifo && !rxBuffer.isFull(); count++) {
            rxBuffer.store_char(dw_iic_receive_data(iic_base));
        }

        if (onReceiveCallback)
            onReceiveCallback(rxBuffer.available());
    }
}

static int i2c_slave_irq(int _irq, void *TwoWireInst)
{
    TwoWire *_i2cInst = reinterpret_cast<TwoWire *>(TwoWireInst);
    _i2cInst->onService();
    return 0;
}

#if WIRE_INTERFACES_COUNT > 0
TwoWire Wire0(0, PIN_IIC0_SDA, PIN_IIC0_SCL);
#endif

#if WIRE_INTERFACES_COUNT > 1
TwoWire Wire1(1, PIN_IIC1_SDA, PIN_IIC1_SCL);
#endif

#if WIRE_INTERFACES_COUNT > 2
TwoWire Wire2(2, PIN_IIC2_SDA, PIN_IIC2_SCL);
#endif

#if WIRE_INTERFACES_COUNT > 3
TwoWire Wire3(3, PIN_IIC3_SDA, PIN_IIC3_SCL);
#endif

#if WIRE_INTERFACES_COUNT > 4
TwoWire Wire4(4, PIN_IIC4_SDA, PIN_IIC4_SCL);
#endif
