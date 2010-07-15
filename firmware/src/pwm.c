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

// Configure the PWM output for the backlight, contrast and button debouncing.
void pwm_init(void) {
	//
	// Configure timer 0 and the associated PWM
	//

	// Configure PWM ports as output
	SETBITS(LCD_PWM_DIR, LCD_BACKLIGHT_PIN | LCD_CONTRAST_PIN);
	
	// Configure Duty Cycle for 0%
	OCR0A = 0x40;
	OCR0B = 0x80;	
	
	// Configure Time/Counter 0 for PWM
	// Compare A & B: Fast PMW mode
	// Waveform Generation:
	// - Top: 0xFF
	// - Update OCR: 0x00
	// - TOV Flag: 0xFF
	// Clock Select: CLK/8
	TCCR0A = 0xA3;
	TCCR0B = 0x02;
	
	//
	// Configure interupt on counter overflow
	//

	// Enable timer 0 overflow interupt
	TIMSK0 |= 0x01;

	// Enable global interupts
	sei();
}

// Set the duty cycle for the backlight
void set_backlight_dc(int value) {
	OCR0A = value;
}

// Set the duty cycle for the contract
void set_contrast_dc(int value) {
	OCR0B = value;
}

// General system timing interupt
ISR(TIMER0_OVF_vect) {
	// Debounce button inputs 
	
}

void input_debounce(volatile uint8_t *Pin, bool *Register, unsigned int *Counter) {
	if (*Pin && (*Counter < 0xFF)) {
		*Counter++;
	} else if (!*Pin && (*Counter > 0x00)) {
		*Counter--;
	}

	if (!*Register && (*Counter == 0xFF)) {
		*Register = true;
	} else if (Register && (*Counter == 0x00)) {
		*Register = false;
	}
}
