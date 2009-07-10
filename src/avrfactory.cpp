/*
 ****************************************************************************
 *
 * simulavr - A simulator for the Atmel AVR family of microcontrollers.
 * Copyright (C) 2001, 2002, 2003   Klaus Rudolph		
 * Copyright (C) 2007 Onno Kortmann
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
 *
 *  $Id$
 */


#include "config.h"
#include "at4433.h"
#include "at8515.h"
#include "atmega48.h"
#include "atmega88.h"
#include "atmega168.h"
#include "atmega128.h"
#include "avrfactory.h"

using namespace std;

/* FIXME: Replace this factory with an automatically and pluggable
factory pattern. (-> AVR devices register themselves.) */

AvrDevice* AvrFactory::makeDevice(const char *device) {
    string c(device); // use copy to transform to lower case

    if (c == "at90s4433") 
        return new AvrDevice_at90s4433();
    if (c == "at90s8515")
        return new AvrDevice_at90s8515();
    if (c == "atmega48")
        return new AvrDevice_atmega48();
    if (c == "atmega88")
        return new AvrDevice_atmega88();
    if (c == "atmega168")
        return new AvrDevice_atmega168();
    if (c == "atmega128")
        return new AvrDevice_atmega128();
    else {
        cerr << "Invalid device specification: " << c << endl;
        exit(1);
    }
}

AvrFactory& AvrFactory::instance() {
    static AvrFactory *f=0;
    if (!f) {
        f=new AvrFactory();
    }

    return *f;
}

AvrFactory::AvrFactory() {}

