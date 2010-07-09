/*

	Project		: Shutter Pod
	File		: pwm.cpp
	Description	: PWM interupts, functions and settings
	Date		: 6/23/2010

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

/*  
 *  File	: pwm.c
 *  Date	: 6/23/2010
 *
 *  Project	: Shutter Pod
 *  Author	: D. Wassenberg
 *  Purpose	: Functions for generating PWM signals
 *
 */

#include "global.h"

int pwm_init() {
		
	return true;
}

void pwm_set_duty_cycle(int port, int pin, int value) {
	
}
