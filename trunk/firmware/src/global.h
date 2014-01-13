/*

	Project		: Shutter Pod
	File		: global.h
	Description	: Global constants, includes, ...
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

#ifndef _GLOBAL_H
#define _GLOBAL_H

// Common Macros for setting & clearing bits
#define SETBITS(x, y)	x |= y
#define CLEARBITS(x, y)	x &= ~y

// constants definitions
#define true	1
#define false	0

//
// I/O Ports
//

// Pins for PWM output
#define LCD_PWM_DIR			DDRD
#define LCD_PWM_MASK		0x60

// Ports for camera triggers
#define	CAMERA_PORT			PORTB
#define	CAMERA_DIR			DDRB
#define	CAMERA_MASK			0x30

#define	SHUTTER_PIN			PORTB5
#define FOCUS_PIN			PORTB4

// Low battery input
#define LOW_BATTERY_PORT	PORTD
#define LOW_BATTERY_DIR		DDRD
#define LOW_BATTERY_MASK	0x80

#define LOW_BATTERY_PIN		PORTD7

// Button input
#define BUTTON_PORT			PORTD
#define BUTTON_PIN			PIND
#define BUTTON_DIR			DDRD
#define BUTTON_MASK			0x1F

#define BUTTON_UP			PIND0
#define BUTTON_RIGHT		PIND1
#define BUTTON_SELECT		PIND2
#define BUTTON_LEFT			PIND3
#define BUTTON_DOWN			PIND4

//
// EVENTS
//

// Button events
enum BUTTON_EVENTS {
	BUTTON_UP_SHORT,
	BUTTON_UP_LONG,

	BUTTON_RIGHT_SHORT,
	BUTTON_RIGHT_LONG,

	BUTTON_SELECT_SHORT,
	BUTTON_SELECT_LONG,

	BUTTON_LEFT_SHORT,
	BUTTON_LEFT_LONG,

	BUTTON_DOWN_SHORT,
	BUTTON_DOWN_LONG,
};

#endif
