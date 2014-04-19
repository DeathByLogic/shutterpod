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

//
// Includes
//

#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "lcd.h"
#include "pwm.h"
#include "camera.h"
#include "misc.h"
#include "menu.h"
#include "fifo.h"

//
// Function constructs
//

void config_lcd(void);
void config_io(void);
void main_menu(int);

//
// Global variables
//

// Global button variables
bool go_button;
unsigned int go_button_counter;

// Global objects
lcd disp(LCD_2_LINE | LCD_5_BY_7);
fifo button_events;

//
// Start of program
//

int main (void) {
	// Configure the io port directions
	config_io();

	// Configure the PWM
	pwm_init();

	// Configure the main timer
	timing_init();

	// Give time for LCD to boot
	delay_ms(250);

	// Configure the LCD
	config_lcd();

	// Print splash screen
	display_splash();

	// Wait for 2 sec
	delay_ms(2000);

	// Enable global interupts
	sei();

	// Display the menu
	display_menu();

	// Goto main menu
	while (true) {
		menu_main();
	}
}

// Setup the i/o ports directions
void config_io(void) {
	// Enable pullups
	MCUCR &= 0x00;

	// Enable pullups on button inputs
	SETBITS(BUTTON_PORT, BUTTON_MASK);

	// Set the button pins as input
	CLEARBITS(BUTTON_DIR, BUTTON_MASK);

	// Configure PWM ports as output
	SETBITS(LCD_PWM_DIR, LCD_PWM_MASK);
	
	// Set camera triggers port direction
	SETBITS(CAMERA_DIR, CAMERA_MASK);

	// Set low battery pin as input
	CLEARBITS(LOW_BATTERY_DIR, LOW_BATTERY_MASK);
}

// Setup the LCD display
void config_lcd(void) {
	// Turn the display on
	disp.display_control(LCD_ON | LCD_UNDERLINE_OFF | LCD_CURSOR_BLINK_ON);

	// Clear the display
	disp.clear_display();

	// Goto the home location
	disp.display_home();

	disp.cursor_blink(false);
}
