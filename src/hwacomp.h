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
#ifndef HWACOMP
#define HWACOMP

#include "hardware.h"
#include "avrdevice.h"
#include "pinatport.h"
#include "rwmem.h"

class HWAcomp: public Hardware {
    protected:
        HWIrqSystem *irqSystem;
        PinAtPort pinAin0;
        PinAtPort pinAin1;

        unsigned char acsr;
        unsigned int irqVec;

    public:
        HWAcomp(AvrDevice *core, HWIrqSystem *irqsys, PinAtPort ain0, PinAtPort ain1, unsigned int irqVec);
        unsigned char GetAcsr();
        void SetAcsr(unsigned char val);
        unsigned int CpuCycle();
        void Reset(); 
        //bool IsIrqFlagSet(unsigned int vec);
        void ClearIrqFlag(unsigned int vec);
};

class RWAcsr: public RWMemoryMembers {
    protected:
        HWAcomp* acomp;
    public:
        RWAcsr(HWAcomp *a) { acomp=a; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};


#endif
