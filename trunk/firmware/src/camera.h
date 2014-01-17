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
#ifndef _CAMERA_H
#define _CAMERA_H

// State definitions
enum CAMERA_MODES {
	MODE_IDLE,
	MODE_LONGEXP,
	MODE_TIMELAPSE,
	MODE_MANUAL
};

enum CAMERA_STATES {
	STATE_IDLE,
	STATE_LE_FOCUS,
	STATE_LE_FOCUS_DELAY,
	STATE_LE_SHUTTER,
	STATE_TL_FOCUS,
	STATE_TL_FOCUS_DELAY,
	STATE_TL_SHUTTER,
	STATE_TL_DELAY,
	STATE_CANCEL
};

#define SEC_TICK  100
#define MIN_TICK  (unsigned long)(60 * SEC_TICK)
#define HOUR_TICK (unsigned long)(60 * MIN_TICK)
#define DAY_TICK  (unsigned long)(24 * HOUR_TICK)

// Function constructs
void set_focus(bool);
void set_shutter(bool);

void timing_init(void);
void shutter(bool);
void focus(bool);

void camera_FSM();

int get_hun_sec(unsigned long);
int get_sec(unsigned long);
int get_min(unsigned long);
int get_hour(unsigned long);
int get_day(unsigned long);

// External global variables
extern CAMERA_MODES camera_mode;

// Delay times for camera
extern unsigned long le_focus_time;
extern unsigned long le_focus_delay;
extern unsigned long le_shutter_time;

extern unsigned long tl_focus_time;
extern unsigned long tl_focus_delay;
extern unsigned long tl_shutter_time;
extern unsigned long tl_shutter_delay;

#endif
