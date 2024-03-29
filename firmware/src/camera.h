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
	MODE_TIMEDELAY
};

enum CAMERA_STATES {
	// Idle State
	STATE_IDLE,
	// Long Exposure States
	STATE_LE_FOCUS,
	STATE_LE_FOCUS_DELAY,
	STATE_LE_SHUTTER,
	// Time-lapse States
	STATE_TL_FOCUS,
	STATE_TL_FOCUS_DELAY,
	STATE_TL_SHUTTER,
	STATE_TL_DELAY,
	// Time Delay States
	STATE_TD_DELAY,
	STATE_TD_FOCUS,
	STATE_TD_FOCUS_DELAY,
	STATE_TD_SHUTTER,
	// Misc States
	STATE_CANCEL
};

#define SEC_TICK  100
#define MIN_TICK  (unsigned long)(60 * SEC_TICK)
#define HOUR_TICK (unsigned long)(60 * MIN_TICK)
#define DAY_TICK  (unsigned long)(24 * HOUR_TICK)

// Function constructs
void set_focus(bool);
void set_shutter(bool);
void set_backlight_level(void);

void timing_init(void);
void timing_deinit(void);
void shutter(bool);
void focus(bool);

void camera_FSM();

uint8_t get_hun_sec(unsigned long *);
uint8_t get_sec(unsigned long *);
uint8_t get_min(unsigned long *);
uint8_t get_hour(unsigned long *);
uint8_t get_day(unsigned long *);

// External global variables
extern CAMERA_MODES camera_mode;

extern unsigned long timeout_count;
extern unsigned long tick_count;
extern unsigned long tick_sum;

#endif
