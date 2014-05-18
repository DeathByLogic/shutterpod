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
#include <avr/interrupt.h>

#include "global.h"
#include "camera.h"
#include "menu.h"
#include "pwm.h"

// Global Variables
CAMERA_MODES camera_mode = MODE_IDLE;

unsigned long timeout_count;

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

void timing_deinit(void) {
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCCR1C = 0x00;

	OCR1AH = 0x00;
	OCR1AL = 0x00;

	// Enable timer 1 overflow interupt
	TIMSK1 = 0x00;
}

// Trigger shutter or focus pin
void set_shutter(bool value) {
	if (value) {
		SETBITS(CAMERA_PORT, 1<<5);
	} else {
		CLEARBITS(CAMERA_PORT, 1<<5);
	}
}

void set_focus(bool value) {
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

	// Backlight timeout
	set_backlight_level();
}

void camera_FSM() {
	static CAMERA_STATES camera_state = STATE_IDLE;
	static unsigned long tick_count;
	static unsigned long tick_sum;

	// Increment tick count
	if (camera_state != STATE_IDLE)
		tick_count++;

	// Check current state and see what to do
	switch (camera_state) {
		case STATE_IDLE:
			// Reset tick count for next state
			tick_count = 0;
			tick_sum = 0;
			
			if (camera_mode == MODE_LONGEXP) {
				// Turn on focus
				set_focus(true);

				// Start long exposure loop
				camera_state = STATE_LE_FOCUS;			
			} else if (camera_mode == MODE_TIMELAPSE) {
				// Turn on focus
				set_focus(true);

				// Start time lapse loop
				camera_state = STATE_TL_FOCUS;
			} else if (camera_mode == MODE_TIMEDELAY) {
				// State time delay loop
				camera_state = STATE_TD_DELAY;
			}

			break;
		//
		// Long Exposure States
		//
		case STATE_LE_FOCUS:
			if (camera_mode == MODE_LONGEXP) {
				if (tick_count >= sys_param.focus_time) {
					// Add tick count to sum
					tick_sum += sys_param.focus_time;

					// Turn off focus
					set_focus(false);

					camera_state = STATE_LE_FOCUS_DELAY;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_LE_FOCUS_DELAY:
			if (camera_mode == MODE_LONGEXP) {
				if (tick_count >= (sys_param.shutter_delay + sys_param.focus_time)) {
					// Add tick count to sum
					tick_sum += sys_param.shutter_delay;

					// Turn on shutter
					set_shutter(true);

					camera_state = STATE_LE_SHUTTER;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_LE_SHUTTER:
			if (camera_mode == MODE_LONGEXP) {
				if (tick_count >= (sys_param.le_shutter_time + sys_param.shutter_delay + sys_param.focus_time)) {
					// Turn off long exposure flag 
					camera_mode = MODE_IDLE;

					// Turn off shutter
					set_shutter(false);

					// Update the LCD display
					update_display();

					camera_state = STATE_IDLE;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		//
		// Timelapse States
		//
		case STATE_TL_FOCUS:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= (sys_param.focus_time + tick_sum)) {
					// Turn off focus
					set_focus(false);

					camera_state = STATE_TL_FOCUS_DELAY;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TL_FOCUS_DELAY:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= (sys_param.shutter_delay + sys_param.focus_time + tick_sum)) {
					// Turn on shutter
					set_shutter(true);

					camera_state = STATE_TL_SHUTTER;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TL_SHUTTER:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= (sys_param.shutter_time + sys_param.shutter_delay + sys_param.focus_time + tick_sum)) {
					// Turn off shutter
					set_shutter(false);

					camera_state = STATE_TL_DELAY;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TL_DELAY:
			if (camera_mode == MODE_TIMELAPSE) {
				if (tick_count >= (sys_param.tl_period + tick_sum)) {
					// Add tick count to sum
					tick_sum += sys_param.tl_period;
					
					if (tick_count >= sys_param.tl_duration) {
						// Turn off time-lapse flag
						camera_mode = MODE_IDLE;

						// Update the LCD display
						update_display();
						
						camera_state = STATE_IDLE;
					} else {
						// Turn on focus
						set_focus(true);

						camera_state = STATE_TL_FOCUS;
					}
				}
			} else {
				camera_state = STATE_CANCEL;
			}
			
			break;
		//
		// Time Delay States
		//
		case STATE_TD_DELAY:
			if (camera_mode == MODE_TIMEDELAY) {
				if (tick_count >= (sys_param.td_delay_time - sys_param.shutter_delay - sys_param.focus_time)) {
					// Turn on focus
					set_focus(true);

					camera_state = STATE_TD_FOCUS;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TD_FOCUS:
			if (camera_mode == MODE_TIMEDELAY) {
				if (tick_count >= (sys_param.td_delay_time - sys_param.shutter_delay)) {
					// Turn off focus
					set_focus(false);

					camera_state = STATE_TD_FOCUS_DELAY;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TD_FOCUS_DELAY:
			if (camera_mode == MODE_TIMEDELAY) {
				if (tick_count >= (sys_param.td_delay_time)) {
					// Turn on shutter
					set_shutter(true);

					camera_state = STATE_TD_SHUTTER;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		case STATE_TD_SHUTTER:
			if (camera_mode == MODE_TIMEDELAY) {
				if (tick_count >= (sys_param.td_delay_time + sys_param.shutter_time)) {
					// Turn off time delay flag 
					camera_mode = MODE_IDLE;

					// Turn off shutter
					set_shutter(false);

					// Update the LCD display
					update_display();

					camera_state = STATE_IDLE;
				}
			} else {
				camera_state = STATE_CANCEL;
			}

			break;
		//
		// Misc States
		//
		case STATE_CANCEL:
			camera_state = STATE_IDLE;
			
			// Disable camera tiggers
			set_shutter(false);
			set_focus(false);

			tick_count = 0;			

			break;
	}
}

void set_backlight_level(void) {
	if (timeout_count < sys_param.timeout_period) {
		timeout_count++;

		if (camera_mode == MODE_LONGEXP or menu_state == SET_DISPLAY_TL_BRIGHTNESS) {
			set_backlight_dc(sys_param.le_brightness_level);
		} else {
			set_backlight_dc(sys_param.brightness_level);
		}
	} else {
		set_backlight_dc(0);
	}
}
