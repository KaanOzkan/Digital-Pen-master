/**
 *  ----------------------------------------------------------------------------
 *  Platform.cpp - AIR430Boost A110x2500 radio platform port implementation.
 *  Copyright (C) 2012-2013 Anaren Microwave, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 * 
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 * 
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *  This example demonstrates usage of the AIR430BoostETSI library which uses
 *  the 430Boost-CC110L AIR Module BoosterPack created by Anaren Microwave, Inc.
 *  and available through the TI eStore, for the European Union.
 *  ----------------------------------------------------------------------------
 *
 *  Note: This file is part of AIR430Boost.
 */
#include "Platform.h"

void A110x2500SpiInit()
{
  // Setup CSn line.
  pinMode (RF_SPI_CSN, OUTPUT);

#if defined(PART_TM4C1233H6PM) || defined (PART_LM4F120H5QR) || defined (PART_TM4C129XNCZAD) || defined (PART_TM4C1294NCPDT)
  // Select the correct SPI port to interface with AIR Booster Pack.
  SPI.setModule(2);
#endif

  /**
   *  Setup the SPI peripheral and SCLK, MISO, and MOSI lines. This is assumed
   *  to be taken care of by the Energia SPI driver.
   *  
   *  Note: It is assumed that the peripheral clock is running at 16MHz. The
   *  radio SPI bus cannot exceed 10MHz.
   */
//  SPI.setClockDivider(SPI_CLOCK_DIV4); //7.5 MHz SPI Clock

  SPI.setDataMode(SPI_MODE0);
  SPI.begin();
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
}

void A110x2500SpiRead(unsigned char address,
                      unsigned char *buffer,
                      unsigned char count)
{
  digitalWrite(RF_SPI_CSN,LOW);

  // Look for CHIP_RDYn from radio.
  pinMode(RF_SPI_MISO, INPUT);
  while (digitalRead(RF_SPI_MISO));
  MAP_PinTypeSPI(PIN_06, PIN_MODE_7);

  // Write the address/command byte.
  SPI.transfer(address);
  
  // Write dummy byte(s) and read response(s).
  for (unsigned char i = 0; i < count; i++)
  {
    buffer[i] = SPI.transfer(0x00);
  }

  // Note: It is assumed that the Energia SPI driver waits until the USCIB0
  // peripheral is done being busy before returning to the caller.

  digitalWrite(RF_SPI_CSN,HIGH);
}

void A110x2500SpiWrite(unsigned char address,
                       const unsigned char *buffer,
                       unsigned char count)
{
  digitalWrite(RF_SPI_CSN,LOW);

  // Look for CHIP_RDYn from radio.
  pinMode(RF_SPI_MISO, INPUT);
  while (digitalRead(RF_SPI_MISO));
  MAP_PinTypeSPI(PIN_06, PIN_MODE_7);
  
  // Write the address/command byte.
  SPI.transfer(address);
  
  // Write dummy byte(s) and read response(s).
  for (unsigned char i = 0; i < count; i++)
  {
    SPI.transfer(buffer[i]);
  }

  // Note: It is assumed that the Energia SPI driver waits until the USCIB0
  // peripheral is done being busy before returning to the caller.

  digitalWrite(RF_SPI_CSN,HIGH);
}

void A110x2500Gdo0Init()
{
  pinMode (RF_GDO0, INPUT);
}

