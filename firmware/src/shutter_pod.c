/*

	Project		: Shutter Pod
	File		: shutter_pod.cpp
	Description	: Main source file for project
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
#include "global.h"
#include "lcd.h"
#include "screen_text.h"

// Function constructs
void config_lcd(void);

// Global variables 
lcd disp(LCD_2_LINE | LCD_5_BY_7);

int main (void) {
	// Configure the LCD
	config_lcd();
	
	// Print splash screen
	disp.print(MSG_SPLASH_1, 16);
	disp.set_display_address(LCD_LINE_2);
	disp.print(MSG_SPLASH_2, 16);
	
	// Wait for 2 sec
	

	// Goto main menu
}

void config_lcd(void) {
	// Turn the display on
	disp.display_control(LCD_ON | LCD_UNDERLINE_OFF | LCD_CURSOR_BLINK_ON);

	// Clear the display
	disp.clear_display();

	// Goto the home location
	disp.display_home();
}

void config_pwm(void) {
	TCCR0A = 0xA3;
	TCCR0B = 0x02;
}
