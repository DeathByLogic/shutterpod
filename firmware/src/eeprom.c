/*

	Project		: Shutter Pod
	File		: eeprom.cpp
	Description	: EEPROM Read / Write
	Date		: 5/4/2014

	Shutter Pod is an open source hardware timer for SLR cameras.
	Copyright (C) 2010 Daniel Wassenberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

//
// Includes
//

#include <avr/eeprom.h>

#include "global.h"
#include "eeprom.h"

//
// Functions
//

void write_sys_param(void) {
	eeprom_write_block(&sys_param, (uint16_t*)SYS_PARAM_ADDRESS, sizeof(sys_param));
}

void update_sys_param(void) {
	eeprom_update_block(&sys_param, (uint16_t*)SYS_PARAM_ADDRESS, sizeof(sys_param));
}

void read_sys_param(void) {
	eeprom_read_block(&sys_param, (uint16_t*)SYS_PARAM_ADDRESS, sizeof(sys_param));
}
