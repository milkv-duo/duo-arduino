/* 
  SPI.h - SPI library for esp32

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include "common.h"
#include "csi_spi.h"

#define SPI_HAS_TRANSACTION

typedef enum {
  SPI_MODE0 = SPI_FORMAT_CPOL0_CPHA0,
  SPI_MODE1 = SPI_FORMAT_CPOL0_CPHA1,
  SPI_MODE2 = SPI_FORMAT_CPOL1_CPHA0,
  SPI_MODE3 = SPI_FORMAT_CPOL1_CPHA1,
} SPIMode;

typedef enum {
  SPI_LSBFIRST,
  SPI_MSBFIRST
} SPIBitOrder;

class SPISettings
{
public:
    SPISettings() :_clock(1000000), _bitOrder(SPI_MSBFIRST), _dataMode(SPI_MODE0) {}
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) :_clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}
    uint32_t _clock;
    uint8_t _bitOrder;
    uint8_t _dataMode;
};

class SPIClass
{
  public:
    SPIClass(uint8_t spiBus = 2);
    ~SPIClass();

    /// @brief SPI begin
    /// @param sckPin Spi SCLK pin
    /// @param misoPin Spi MISO pin, -1 not used
    /// @param mosiPin Spi MOSI pin
    /// @param csPin Spi CS pin, -1 not used
    void begin(int8_t sckPin, int8_t misoPin, int8_t mosiPin, int8_t csPin);
    inline void begin()
    {
        return begin( PIN_SPI2_SCK, PIN_SPI2_MISO, PIN_SPI2_MOSI, PIN_SPI2_CS);
    }
    void end();

    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t dataMode);
    void setFrequency(uint32_t freq);

    void beginTransaction(SPISettings settings);
    void endTransaction(void);
    void transfer(void * data, uint32_t size);
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);

    void transferBytes(void* data, void* out, uint32_t size);

  private:
    uint8_t _idx;
    bool _initialized;

    csi_spi_t _spiBus;

    csi_dma_ch_t tx_dma; 
    csi_dma_ch_t rx_dma;

    volatile uint8_t spi_flag = 0;

    int8_t _sckPin, _mosiPin, _misoPin, _csPin;

    bool _inTransaction;
    bool _lock;


    SPISettings _setting;
};

extern SPIClass SPI;

#endif /* _SPI_H_INCLUDED */
