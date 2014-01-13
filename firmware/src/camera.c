/*

	Project		: Shutter Pod
	File		: camera.c
	Description	: Timing and FSM for camera tiggers
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

// Includes
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "camera.h"

// Global Variables
CAMERA_MODES camera_mode = MODE_IDLE;

// Delay times for camera
unsigned long le_focus_time = 18;
unsigned long le_focus_delay = 90;
unsigned long le_shutter_time = 180;

unsigned long tl_focus_time = 18;
unsigned long tl_focus_delay = 90;
unsigned long tl_shutter_time = 18;
unsigned long tl_shutter_delay = 90;

// Configure timer 2 for delays
void timing_init(void) {
	//
	// Configure Time/Counter 2 for counting up
	// with an overflow interupt giving a freq
	// of ~61 Hz
	//

	// Clock Select: CLK/1024
	TCCR2A = 0x00;
	TCCR2B = 0x07;

	// Enable timer 2 overflow interupt
	TIMSK2 |= 0x01;
}

// Trigger shutter or focus pin
void set_shutter(bool value) {
//	SHUTTER_PIN = value?1:0;
	
	if (value) {
		SETBITS(CAMERA_PORT, 1<<5);
	} else {
		CLEARBITS(CAMERA_PORT, 1<<5);
	}
}

void set_focus(bool value) {
//	FOCUS_PIN = value;
	
	if (value) {
		SETBITS(CAMERA_PORT, 1<<4);
	} else {
		CLEARBITS(CAMERA_PORT, 1<<4);
	}
}

//
// State machine that controls the timing and
// state of the focus and shutter pins.
//
ISR (TIMER2_OVF_vect) {
	// Run the camera state machine
	camera_FSM();
	
	// Debounce input buttons
	//debouce_buttons();
}

void camera_FSM() {
	static CAMERA_STATES camera_state = STATE_IDLE;
	static unsigned long tick_count;

	// Increment tick count
	if (camera_state != STATE_IDLE)
		tick_count++;

	// Check current state and see what to do
	switch (camera_state) {
		case STATE_IDLE:
			if (camera_mode == MODE_LONGEXP) {
				// Turn on focus
				set_focus(true);

				// Start long exposure loop
				camera_state = STATE_LE_FOCUS;

				// Reset tick count for next state
				tick_count = 0;
			} else if (camera_mode == MODE_TIMELAPSE) {
				// Turn on focus
				set_focus(true);

				// Start time lapse loop
				camera_state = STATE_TL_FOCUS;

				// Reset tick count for next state
				tick_count = 0;
			}

			break;
		case STATE_LE_FOCUS:
			if (camera_mode == MODE_LONGEXP) {
				if (tick_count >= le_focus_time) {
					// Turn off focus
					set_focus(false);

					camera_state = STATE_LE_FOCUS_DELAY;

					// Reset tick count for next state
					tick_count = 0;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_LE_FOCUS_DELAY:
			if (camera_mode == MODE_LONGEXP) {
				if (tick_count >= le_focus_delay) {
					// Turn on shutter
					set_shutter(true);

					camera_state = STATE_LE_SHUTTER;

					// Reset tick count for next state
					tick_count = 0;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_LE_SHUTTER:
			if (camera_mode == MODE_LONGEXP) {
				if (tick_count >= le_shutter_time) {
					// Turn off long exposure flag 
					camera_mode = MODE_IDLE;

					// Turn off shutter
					set_shutter(false);

					camera_state = STATE_IDLE;

					// Reset tick count for next state
					tick_count = 0;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TL_FOCUS:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= tl_focus_time) {
					// Turn off focus
					set_focus(false);

					camera_state = STATE_TL_FOCUS_DELAY;

					tick_count = 0;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TL_FOCUS_DELAY:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= tl_focus_delay) {
					// Turn on shutter
					set_shutter(true);

					camera_state = STATE_TL_SHUTTER;

					tick_count = 0;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TL_SHUTTER:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= tl_shutter_time) {
					// Turn off shutter
					set_shutter(false);

					camera_state = STATE_TL_DELAY;

					tick_count = 0;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TL_DELAY:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= tl_shutter_delay) {
					// Turn on focus
					set_focus(true);

					camera_state = STATE_TL_FOCUS;

					tick_count = 0;
				}
			} else {
				camera_state = STATE_CANCEL;
			}
			
			break;
		case STATE_CANCEL:
			camera_state = STATE_IDLE;
			
			set_shutter(false);
			set_focus(false);

			tick_count = 0;			

			break;
	}
}