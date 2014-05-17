/*

	Project		: Shutter Pod
	File		: low_power.cpp
	Description	: Low power and sleep modes
	Date		: 5/6/2014

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
#include <avr/sleep.h>

#include "global.h"
#include "low_power.h"
#include "pwm.h"
#include "camera.h"
#include "lcd.h"

bool enter_pwr_dwn = false;

void idle(void) {
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	sleep_cpu();
	sleep_disable();
}

void power_down(void) {
	disable_devices();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_cpu();
	sleep_disable();

	enable_devices();
}

void disable_devices(void) {
	// Turn off LCD display
	disp.display_on(false);

	// Disable PWM functions
	pwm_deinit();

	// Disable LCD outputs
	CLEARBITS(CMD_DIR, RS_PIN | RW_PIN | EN_PIN);

	// Disable PWM outputs
	CLEARBITS(LCD_PWM_DIR, LCD_PWM_MASK);
	
	// Disable camera triggers outputs
	CLEARBITS(CAMERA_DIR, CAMERA_MASK);
}

void enable_devices(void) {
	// Enable PWM outputs
	SETBITS(LCD_PWM_DIR, LCD_PWM_MASK);
	
	// Enable camera triggers outputs
	SETBITS(CAMERA_DIR, CAMERA_MASK);

	// Enable LCD outputs
	SETBITS(CMD_DIR, RS_PIN | RW_PIN | EN_PIN);
	
	// Turn on LCD display
	disp.display_on(true);

	// Enable PWM functions
	pwm_init();
}
