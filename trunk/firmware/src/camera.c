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
#include <math.h>
#include <avr/interrupt.h>

#include "global.h"
#include "camera.h"
#include "pwm.h"

// Global Variables
CAMERA_MODES camera_mode = MODE_IDLE;

// Configure timer 2 for delays
void timing_init(void) {
	// Configure Time/Counter 1 for PWM
	// Fast PWM Mode
	// Waveform Generation:
	// - Top: 0x0270
	// - Update OCR: 0x00
	// - TOV Flag: Top
	// Clock Select: CLK/256
	TCCR1A = 0x03;
	TCCR1B = 0x1C;
	TCCR1C = 0x00;

	OCR1AH = 0x02;
	OCR1AL = 0x70;

	// Enable timer 1 overflow interupt
	TIMSK1 |= 0x01;
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
ISR (TIMER1_OVF_vect) {
	// Run the camera state machine
	camera_FSM();
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
				
				// Set backlight level
				set_backlight_level();

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
				if (tick_count >= sys_param.focus_time) {
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
				if (tick_count >= sys_param.shutter_delay) {
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
				if (tick_count >= sys_param.le_shutter_time) {
					// Turn off long exposure flag 
					camera_mode = MODE_IDLE;

					// Turn off shutter
					set_shutter(false);
					
					// Set backlight level
					set_backlight_level();
					
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
				if (tick_count >= sys_param.focus_time) {
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
				if (tick_count >= sys_param.shutter_delay) {
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
				if (tick_count >= sys_param.shutter_time) {
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
				if (tick_count >= sys_param.tl_period) {
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
			
			// Disable camera tiggers
			set_shutter(false);
			set_focus(false);

			// Set backlight level
			set_backlight_level();

			tick_count = 0;			

			break;
	}
}

void  set_backlight_level(void) {
	if (camera_mode == MODE_LONGEXP) {
		set_backlight_dc(sys_param.le_brightness_level);
	} else {
		set_backlight_dc(sys_param.brightness_level);
	}
}
