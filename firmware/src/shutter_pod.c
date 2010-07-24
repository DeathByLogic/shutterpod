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

// Temp
extern bool le_flag;
extern bool tl_flag;

// Includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include "global.h"

#include "lcd.h"
#include "pwm.h"
#include "camera.h"
#include "screen_text.h"

// Function constructs
void config_lcd(void);

// Global button variables
bool go_button;
unsigned int go_button_counter;

// Global variables 
lcd disp(LCD_2_LINE | LCD_5_BY_7);

int main (void) {
	DDRB = 0x30;
	
	// Configure the LCD
	config_lcd();

	// Configure the PWM
	pwm_init();

	// Configure the main timer
	timing_init();

	// Enable global interupts
	sei();

	// Print splash screen
	disp.print(MSG_SPLASH_1, 16);
	disp.set_display_address(LCD_LINE_2);
	disp.print(MSG_SPLASH_2, 16);
	
	// Wait for 2 sec

	// Goto main menu

	while(true) {
		//le_flag = false;
		//tl_flag = true;		
		//focus(true);
		asm("NOP");
		//focus(false);
		asm("NOP");
	}
}

void config_lcd(void) {
	// Turn the display on
	disp.display_control(LCD_ON | LCD_UNDERLINE_OFF | LCD_CURSOR_BLINK_ON);

	// Clear the display
	disp.clear_display();

	// Goto the home location
	disp.display_home();
}
