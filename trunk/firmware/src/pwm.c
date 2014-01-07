/*

	Project		: Shutter Pod
	File		: pwm.cpp
	Description	: PWM interupts, functions and settings
	Date		: 7/8/2010

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
#include "pwm.h"
#include "lcd.h"
#include "fifo.h"
#include "debounce.h"

//Function Constructors
void call_back(int flag, bool value);

// Global Variables
extern fifo button_events;

debounce btn_db_up(&call_back, BUTTON_UP_SHORT, BUTTON_UP_LONG);
debounce btn_db_down(&call_back, BUTTON_DOWN_SHORT, BUTTON_DOWN_LONG);
debounce btn_db_left(&call_back, BUTTON_LEFT_SHORT, BUTTON_LEFT_LONG);
debounce btn_db_right(&call_back, BUTTON_RIGHT_SHORT, BUTTON_RIGHT_LONG);
debounce btn_db_select(&call_back, BUTTON_SELECT_SHORT, BUTTON_SELECT_LONG);

// Configure the PWM output for the backlight, contrast and button debouncing.
void pwm_init(void) {
	// Configure Duty Cycle for 50%
	OCR0A = 0xFF;//0x80;
	OCR0B = 0x80;//0x80;	
	
	// Configure Time/Counter 0 for PWM
	// Compare A & B: Fast PMW mode
	// Waveform Generation:
	// - Top: 0xFF
	// - Update OCR: 0x00
	// - TOV Flag: 0xFF
	// Clock Select: CLK/8
	TCCR0A = 0xA3;
	TCCR0B = 0x02;

	// Enable timer 0 overflow interupt
	TIMSK0 |= 0x01;
}

// Set the duty cycle for the backlight
void set_backlight_dc(int value) {
	OCR0B = value;
}

// Set the duty cycle for the contract
void set_contrast_dc(int value) {
	OCR0A = value;
}

// General system timing interupt
ISR(TIMER0_OVF_vect) {
	btn_db_up.update((bool)(BUTTON_PIN & (1 << BUTTON_UP)));
	btn_db_down.update((bool)(BUTTON_PIN & (1 << BUTTON_DOWN)));
	btn_db_left.update((bool)(BUTTON_PIN & (1 << BUTTON_LEFT)));
	btn_db_right.update((bool)(BUTTON_PIN & (1 << BUTTON_RIGHT)));
	btn_db_select.update((bool)(BUTTON_PIN & (1 << BUTTON_SELECT)));
}

void call_back(int flag, bool value) {
	if (value == false) {
		button_events.push(flag);
	}
}
