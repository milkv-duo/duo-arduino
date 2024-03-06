/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "Arduino.h"
#include "csi_pin.h"

#include "SPI.h"

#define SPI_PARAM_LOCK() { \
    do {} while (_lock); \
    _lock = true; \ 
}

#define SPI_PARAM_UNLOCK()  _lock = false;

extern dev_pin_map_t spi_sck_map[];
extern dev_pin_map_t spi_miso_map[];
extern dev_pin_map_t spi_mosi_map[];
extern dev_pin_map_t spi_cs_map[];

SPIClass::SPIClass(uint8_t spiBus)
    : _idx(spiBus),
    _sckPin(-1),
    _mosiPin(-1),
    _misoPin(-1),
    _csPin(-1),
    _inTransaction(false),
    _lock(false),
    _setting(SPISettings()),
    _initialized(false)
{
}

SPIClass::~SPIClass()
{
    end();
}

void SPIClass::begin(int8_t sckPin, int8_t misoPin, int8_t mosiPin, int8_t csPin)
{
    const dev_pin_map_t* sck_pin = target_pin_number_to_dev(sckPin, spi_sck_map, _idx);
    const dev_pin_map_t* miso_pin = target_pin_number_to_dev(misoPin, spi_miso_map, _idx);
    const dev_pin_map_t* mosi_pin = target_pin_number_to_dev(mosiPin, spi_mosi_map, _idx);
    const dev_pin_map_t* cs_pin = target_pin_number_to_dev(csPin, spi_cs_map, _idx);

    if (sck_pin == NULL || miso_pin == NULL || mosi_pin == NULL || cs_pin == NULL) {
        pr_err("pin GPIO (%d, %d, %d, %d) are not used as SPI func\n", sckPin, misoPin, mosiPin, csPin);
        return;
    }

    if (csi_pin_set_mux(sck_pin->name, sck_pin->func)) {
        pr_err("pin GPIO %d fails to config as spi_csk func\n", sckPin);
        return;
    }

    if (csi_pin_set_mux(miso_pin->name, miso_pin->func)) {
        pr_err("pin GPIO %d fails to config as spi_miso func\n", misoPin);
        return;
    }

    if (csi_pin_set_mux(mosi_pin->name, mosi_pin->func)) {
        pr_err("pin GPIO %d fails to config as spi_mosi func\n", mosiPin);
        return;
    }

    if (csi_pin_set_mux(cs_pin->name, cs_pin->func)) {
        pr_err("pin GPIO %d fails to config as spi_cs func\n", csPin);
        return;
    }

    SPI_PARAM_LOCK();

    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _csPin = csPin;

    if (csi_spi_init(&_spiBus, _idx) != CSI_OK) {
        goto _fail;
    }

    if (csi_spi_mode(&_spiBus, SPI_MASTER) != CSI_OK) {
        goto _fail;
    }

    csi_spi_cp_format(&_spiBus, (csi_spi_cp_format_t)_setting._dataMode);

    csi_spi_baud(&_spiBus, _setting._clock);

#ifdef USE_SPI_DMA
    csi_spi_attach_callback(&_spiBus, (void*)spi_callback_func,  (void *)&spi_flag);

    csi_spi_link_dma(&_spiBus, &tx_dma, &rx_dma);
#endif

    _initialized = true;
    SPI_PARAM_UNLOCK();
    return;

_fail:
    pr_err("spi init failed\r\n");
    csi_spi_uninit(&_spiBus);
    SPI_PARAM_UNLOCK();
}

void SPIClass::end()
{
    SPI_PARAM_LOCK();

    _initialized = false;
    csi_spi_detach_callback(&_spiBus);
    csi_spi_uninit(&_spiBus);

    SPI_PARAM_UNLOCK();
}

void SPIClass::setFrequency(uint32_t freq)
{
    SPI_PARAM_LOCK();

    if (_setting._clock != freq) {
        _setting._clock = freq;
        csi_spi_baud(&_spiBus, _setting._clock);
    }

    SPI_PARAM_UNLOCK();
}

void SPIClass::setDataMode(uint8_t dataMode)
{
    SPI_PARAM_LOCK();

    if (_setting._dataMode != dataMode) {
        _setting._dataMode = dataMode;

        csi_spi_cp_format(&_spiBus, (csi_spi_cp_format_t)_setting._dataMode);
    }

    SPI_PARAM_UNLOCK();
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
    SPI_PARAM_LOCK();

    if (_setting._bitOrder != bitOrder) {
        _setting._bitOrder = bitOrder;

        /* NOT SUPPORTED */
        pr_err("Only support SPI_MSBFIRST \r\n");
    }

    SPI_PARAM_UNLOCK();
}

void SPIClass::beginTransaction(SPISettings settings)
{
    if (_setting._bitOrder != settings._bitOrder) {
        setBitOrder(settings._bitOrder);
    }

    if (_setting._dataMode != settings._dataMode) {
        setDataMode(settings._dataMode);
    }

    if (_setting._clock != settings._clock) {
        setFrequency(settings._clock);
    }

    SPI_PARAM_LOCK();
    _inTransaction = true;
}

void SPIClass::endTransaction()
{
    if (_inTransaction) {
        _inTransaction = false;

        SPI_PARAM_UNLOCK(); // <-- Im not sure should it be here or right after spiTransaction()
    }
}

uint8_t SPIClass::transfer(uint8_t data)
{
    uint8_t t = data, r = 0xFF;

    csi_spi_frame_len(&_spiBus, SPI_FRAME_LEN_8);
    csi_spi_select_slave(&_spiBus, 0);
    csi_spi_send_receive(&_spiBus, &t, &r, 1, 10000);

    return r;
}

uint16_t SPIClass::transfer16(uint16_t data)
{
    uint8_t t[2];
    uint16_t r = 0xFFFF;
    t[0]=(uint8_t)((data) >> 8);
    t[1]=(uint8_t)((data) >> 0);

    csi_spi_frame_len(&_spiBus, SPI_FRAME_LEN_8);
    csi_spi_select_slave(&_spiBus, 0);
    csi_spi_send_receive(&_spiBus, &t, &r, 2, 10000);

    return r;
}

void SPIClass::transfer(void * data, uint32_t size) 
{ 
    transferBytes((uint8_t *)data, (uint8_t *)data, size); 
}

/**
 * @param data uint8_t * data buffer. can be NULL for Read Only operation
 * @param out  uint8_t * output buffer. can be NULL for Write Only operation
 * @param size uint32_t
 */
void SPIClass::transferBytes(void* data, void* out, uint32_t size)
{
    
    csi_spi_frame_len(&_spiBus, SPI_FRAME_LEN_8);
    csi_spi_select_slave(&_spiBus, 0);

#ifdef USE_SPI_DMA
    spi_flag = 1;
    csi_spi_send_receive_async(&_spiBus, (void*)data, (void*)data, size);
    while (spi_flag);
#else
    csi_spi_send_receive(&_spiBus, (void*)data, (void*)data, size, 10000);
#endif
}

SPIClass SPI(2);
