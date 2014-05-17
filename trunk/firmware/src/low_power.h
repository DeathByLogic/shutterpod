/*

	Project		: Shutter Pod
	File		: low_power.h
	Description	: Low power and sleep header
	Date		: 5/6/2014

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
#ifndef _LOW_POWER_H
#define _LOW_POWER_H

void power_down(void);
void idle(void);
void disable_devices(void);
void enable_devices(void);

extern bool enter_pwr_dwn;

#endif
