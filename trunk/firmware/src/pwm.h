/*

	Project		: Shutter Pod
	File		: pwm.h
	Description	: PWM header file
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

// Pins for PWM output
#define LCD_BACKLIGHT_PIN	0x20
#define LCD_CONTRAST_PIN	0x40
#define LCD_PWM_DIR			DDRD

// function constructs
void pwm_init(void);
void set_backlight_dc(void);
void set_contrast_dc(void);
void input_debounce(volatile uint8_t *, bool *, unsigned int *);

extern bool go_button;
extern unsigned int go_button_counter;
