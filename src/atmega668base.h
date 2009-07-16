 /*
 ****************************************************************************
 *
 * simulavr - A simulator for the Atmel AVR family of microcontrollers.
 * Copyright (C) 2001, 2002, 2003   Klaus Rudolph       
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ****************************************************************************
 */
#ifndef ATMEGA668
#define ATMEGA668

#include "avrdevice.h"
#include "hardware.h"
#include "hwmega48extirq.h"
#include "hwuart.h"
#include "hwad.h"
#include "hwport.h"
#include "hwspi.h"
#include "hwtimer.h"
#include "hwmegax8timer.h"
#include "hwmegax8timerirq.h"

/*! AVRDevice class for ATMega48/88/168/328
  \todo This device isn't completely implemented. timer1 is missed. There is no
  boot loader section support for >= ATMega88, only normal interrupt vector
  start address supported. (and maybe more ...) */
class AvrDevice_atmega668base: public AvrDevice {
    
    protected:
        Pin                 aref;       //!< analog reference pin
        Pin                 adc6;       //!< adc channel 6 input pin
        Pin                 adc7;       //!< adc channel 7 input pin
        HWPort              portb;      //!< port B
        HWPort              portc;      //!< port C
        HWPort              portd;      //!< port D
        HWPrescaler         prescaler;  //!< prescaler unit for timer 0
        HWMega48ExtIrq*     extirq;     //!< external interrupt unit
        HWAdmux             admux;      //!< adc multiplexer unit
        HWAd*               ad;         //!< adc unit
        HWSpi*              spi;        //!< spi unit
        HWUsart*            usart0;     //!< usart 0 unit
        HWMegaX8TimerIrq*   timerIrq0;  //!< timer interrupt unit for timer 0
        HWMegaX8Timer0*     timer0;     //!< timer 0 unit
        
    public:
        
        /*! Creates the device for ATMega48/88/168/328
          @param ram_bytes how much SRAM does the device own
          @param flash_bytes how much flash memory does the device own
          @param ee_bytes how much EEPROM space does the device own */
        AvrDevice_atmega668base(unsigned ram_bytes, unsigned flash_bytes,
                                unsigned ee_bytes );
        
        ~AvrDevice_atmega668base();
        
        /*! read support of Rampz register
        
          This method shows a warning message, because this hardware feature
          isn't available on ATMega48/88/168/328
          @return allways 0 */
        unsigned char GetRampz(); 
        
        /*! write support of Rampz register
        
          This method shows a warning message, because this hardware feature
          isn't available on ATMega48/88/168/328
          @param val value to write */
        void SetRampz(unsigned char val);
};

//! AVR device class for ATMega328, see AvrDevice_atmega668base.
class AvrDevice_atmega328: public AvrDevice_atmega668base {
    public:
        //! Creates the device for ATMega328, see AvrDevice_atmega668base.
        AvrDevice_atmega328() : AvrDevice_atmega668base(2 * 1024, 32 * 1024, 1024) {}
};

//! AVR device class for ATMega168, see AvrDevice_atmega668base.
class AvrDevice_atmega168: public AvrDevice_atmega668base {
    public:
        //! Creates the device for ATMega168, see AvrDevice_atmega668base.
        AvrDevice_atmega168() : AvrDevice_atmega668base(1024, 16 * 1024, 512) {}
};

//! AVR device class for ATMega88, see AvrDevice_atmega668base.
class AvrDevice_atmega88:public AvrDevice_atmega668base {
    public:
        //! Creates the device for ATMega88, see AvrDevice_atmega668base.
        AvrDevice_atmega88() : AvrDevice_atmega668base(1024, 8 * 1024, 512) {}
};

//! AVR device class for ATMega48, see AvrDevice_atmega668base.
class AvrDevice_atmega48:public AvrDevice_atmega668base {
    public:
        //! Creates the device for ATMega48, see AvrDevice_atmega668base.
        AvrDevice_atmega48() : AvrDevice_atmega668base(512, 4 * 1024, 256) {}
};

#endif
