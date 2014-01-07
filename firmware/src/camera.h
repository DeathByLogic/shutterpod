/*

	Project		: Shutter Pod
	File		: camera.h
	Description	: Timing and FSM header file
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

// State definitions
#define STATE_IDLE				0x00

#define STATE_LE_FOCUS			0x01
#define STATE_LE_FOCUS_DELAY	0x02
#define STATE_LE_SHUTTER		0x03

#define STATE_TL_FOCUS			0x10
#define STATE_TL_FOCUS_DELAY	0x20
#define STATE_TL_SHUTTER		0x30
#define	STATE_TL_DELAY			0x40

#define STATE_CANCEL			0xFF

// Function constructs 
void timing_init(void);
void shutter(bool);
void focus(bool);

// External global variables
extern bool le_flag;
extern bool tl_flag;

// Delay times for camera
extern unsigned long le_focus_time;
extern unsigned long le_focus_delay;
extern unsigned long le_shutter_time;

extern unsigned long tl_focus_time;
extern unsigned long tl_focus_delay;
extern unsigned long tl_shutter_time;
extern unsigned long tl_shutter_delay;
