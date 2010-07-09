/*

File		: shutter_pod.c
Date		: 6/23/2010

Project		: Shutter Pod
Description	: Main source file for project



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
