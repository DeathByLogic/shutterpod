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

// Global variables
unsigned long le_focus_time = 300;		//0;
unsigned long le_focus_delay = 1500;	//0;
unsigned long le_shutter_time = 3000;	//0;

unsigned long tl_focus_time = 300;		//0;
unsigned long tl_focus_delay = 1500;	//0;
unsigned long tl_shutter_time = 300;	//0;
unsigned long tl_shutter_delay = 1500;	//0;

bool le_flag = true;
bool tl_flag = false;

// Configure timer 2 for delays
void timing_init(void) {
	// Set camera triggers port direction
	SETBITS(CAMERA_DIR, SHUTTER_PIN | FOCUS_PIN);

	// Configure Time/Counter 2 for counting up
	// Clock Select: CLK/1024
	TCCR2A = 0x00;
	TCCR2B = 0x07;

	// Enable timer 2 overflow interupt
	TIMSK2 |= 0x01;
}

// Trigger shutter or focus pin
void set_shutter(bool value) {
	if (value) {
		SETBITS(CAMERA_PORT, SHUTTER_PIN);
	} else {
		CLEARBITS(CAMERA_PORT, SHUTTER_PIN);
	}
}

void set_focus(bool value) {
	if (value) {
		SETBITS(CAMERA_PORT, FOCUS_PIN);
	} else {
		CLEARBITS(CAMERA_PORT, FOCUS_PIN);
	}
}

//
// State machine that controls the timing and
// state of the focus and shutter pins.
//
ISR (TIMER2_OVF_vect) {
	// Current and next state for FSM
	static int current_state = STATE_IDLE;
	static int next_state;

	static unsigned long tick_count;

	// Increment tick count
	if (current_state != STATE_IDLE)
		tick_count++;

	// Check current state and see what to do
	switch (current_state) {
		case STATE_IDLE:
			if (le_flag) {
				// Turn on focus
				set_focus(true);

				// Start long exposure loop
				next_state = STATE_LE_FOCUS;

				// Reset tick count for next state
				tick_count = 0;
			} else if (tl_flag) {
				// Turn on focus
				set_focus(true);

				// Start time lapse loop
				next_state = STATE_TL_FOCUS;

				// Reset tick count for next state
				tick_count = 0;
			}

			break;
		case STATE_LE_FOCUS:
			if (tick_count >= le_focus_time) {
				// Turn off focus
				set_focus(false);

				next_state = STATE_LE_FOCUS_DELAY;

				// Reset tick count for next state
				tick_count = 0;
			}

			break;
		case STATE_LE_FOCUS_DELAY:
			if (tick_count >= le_focus_delay) {
				// Turn on shutter
				set_shutter(true);

				next_state = STATE_LE_SHUTTER;

				// Reset tick count for next state
				tick_count = 0;
			}

			break;
		case STATE_LE_SHUTTER:
			if (tick_count >= le_shutter_time) {
				// Turn off long exposure flag 
				le_flag = false;

				// Turn off shutter
				set_shutter(false);

				next_state = STATE_IDLE;

				// Reset tick count for next state
				tick_count = 0;
			}

			break;
		case STATE_TL_FOCUS:
			if (!tl_flag) {
				//  Turn off focus
				set_focus(false);

				next_state = STATE_IDLE;

				tick_count = 0;
			} else if (tick_count >= tl_focus_time) {
				// Turn off focus
				set_focus(false);

				next_state = STATE_TL_FOCUS_DELAY;

				tick_count = 0;
			}

			break;
		case STATE_TL_FOCUS_DELAY:
			if (!tl_flag) {
				next_state = STATE_IDLE;

				tick_count = 0;
			} else if (tick_count >= tl_focus_delay) {
				// Turn on shutter
				set_shutter(true);

				next_state = STATE_TL_SHUTTER;

				tick_count = 0;
			}

			break;
		case STATE_TL_SHUTTER:
			if (!tl_flag) {
				// Turn off shutter
				set_shutter(false);

				next_state = STATE_IDLE;

				tick_count = 0;
			} else if (tick_count >= tl_shutter_time) {
				// Turn off shutter
				set_shutter(false);

				next_state = STATE_TL_DELAY;

				tick_count = 0;
			}

			break;
		case STATE_TL_DELAY:
			if (!tl_flag) {
				next_state = STATE_IDLE;

				tick_count = 0;
			} else if (tick_count >= tl_shutter_delay) {
				// Turn on focus
				set_focus(true);

				next_state = STATE_TL_FOCUS;

				tick_count = 0;
			}
			
			break;
	}
	
	// Go to the next state
	current_state = next_state;
}
