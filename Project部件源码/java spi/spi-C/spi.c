/*
 * wiringPiSPI.c:
 *	Simplified SPI access routines
 *	Copyright (c) 2012 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"


// The SPI bus parameters
//	Variables as they need to be passed as pointers later on

const static uint8_t     spiMode  = 0 ;
const static uint8_t     spiBPW   = 8 ;
const static uint16_t    spiDelay = 0 ;


/*
 * wiringPiSPIDataRW:
 *	Write and Read a block of data over the SPI bus.
 *	Note the data ia being read into the transmit buffer, so will
 *	overwrite it!
 *	This is also a full-duplex operation.
 *********************************************************************************
 */

int SPIDataRW (int fd, int speed,unsigned char *data, int len)
{
  struct spi_ioc_transfer spi ;

  spi.tx_buf        = (unsigned long)data ;
  spi.rx_buf        = (unsigned long)data ;
  spi.len           = len ;
  spi.delay_usecs   = spiDelay ;
  spi.speed_hz      = speed;
  spi.bits_per_word = spiBPW ;

  return ioctl (fd, SPI_IOC_MESSAGE(1), &spi) ;
}


/*
 * wiringPiSPISetup:
 *	Open the SPI device, and set it up, etc.
 *********************************************************************************
 */
int SPISetup (char *spiPath, int speed)
{
  int fd ;

  if ((fd = open (spiPath, O_RDWR)) < 0)
    return printf("Unable to open SPI device: %s\n", strerror (errno)) ;


  if (ioctl (fd, SPI_IOC_WR_MODE, &spiMode)         < 0)
    return printf ("SPI Mode Change failure: %s\n", strerror (errno)) ;
  
  if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
    return printf ("SPI BPW Change failure: %s\n", strerror (errno)) ;

  if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)   < 0)
    return printf ("SPI Speed Change failure: %s\n", strerror (errno)) ;

  return fd ;
}

int jack(char* str){
	printf(str);
}