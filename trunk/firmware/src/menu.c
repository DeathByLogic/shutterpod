/*

	Project		: Shutter Pod
	File		: menu.cpp
	Description	: Menu tree logic
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

#include "global.h"
#include "lcd.h"
#include "fifo.h"
#include "menu.h"
#include "misc.h"
#include "screen_text.h"

// External variables
extern lcd disp;
extern fifo button_events;

// dispay the splash menu
void display_splash() {
	// Clear the display and go home
	disp.clear_display();

	// Print splash screen
	disp.print(MSG_SPLASH_1, 16);
	disp.set_display_address(LCD_LINE_2);
	disp.print(MSG_SPLASH_2, 16);
}

/*
menu_type::menu_type(char * menu_text, char * item_text, menu_type * up, menu_type * down, menu_type * left, menu_type * right) {
	m_txt = menu_text;
	i_txt = item_text;

	n_up = up;
	n_down = down;
	n_left = left;
	n_right = right;
};*/

//
// Main Menu FSM
//

void menu_main(void) {
	int current_state = MENU_MAIN_SETTINGS;

	int button_event;

	print_menu(MSG_MAIN_MENU, MSG_SETTINGS_OPT);

	while (true) {
		// Pull next button event off stack
		button_event = button_events.pop();

		switch (current_state) {
			case MENU_MAIN_SETTINGS:
				current_state = menu_main_settings(button_event);				

				break;
			case MENU_MAIN_LONGEXP:
				current_state = menu_main_longexp(button_event);

				break;
			case MENU_MAIN_TIMELSP:
				current_state = menu_main_timelsp(button_event);

				break;
			case MENU_MAIN_MANUAL:
				current_state = menu_main_manual(button_event);

				break;
		}

	}
}

int menu_main_longexp(int button_event) {
	
	switch (button_event) {
		case BUTTON_LEFT_SHORT:
		case BUTTON_LEFT_LONG:
			return MENU_MAIN_TIMELSP;
			
			break;
		case BUTTON_RIGHT_SHORT:
		case BUTTON_RIGHT_LONG:
			return MENU_MAIN_SETTINGS;
		
			break;
		case BUTTON_SELECT_SHORT:
		case BUTTON_SELECT_LONG:
			return MENU_MAIN_LONGEXP;
						
			break;
	}
	
	return MENU_MAIN_LONGEXP;
}

int menu_main_timelsp(int button_event) {

	return MENU_TIMELSP;
}

int menu_main_manual(int button_event) {

	return MENU_MANUAL;
}

int menu_main_settings(int button_event) {

	return MENU_MANUAL;
}

unsigned int get_time(unsigned int time) {
	#define STATE_MILI_SEC	0x01
	#define STATE_SEC		0x02
	#define STATE_MIN		0x03
	#define	STATE_HOUR		0x04
	#define	STATE_DAY		0x05
	
	// Time storage
	char miliseconds;
	char seconds;
	char minutes;
	char hours;
	char days;
	
	// Current and next state
	int current_state;
	int next_state;

	//
	int button_event;
	bool exit = false;

	// Configure LCD Settings

	while (!exit) {
		button_event = button_events.pop();
		
		if (button_event > 0) {
			switch (current_state) {
				case STATE_MILI_SEC:
					switch (button_event) {
						case BUTTON_UP_SHORT:
							miliseconds += 1;

							break;
						case BUTTON_DOWN_SHORT:
							miliseconds -= 1;
											
							break;
						case BUTTON_LEFT_SHORT:
							next_state = STATE_SEC;

							break;
					
						case BUTTON_SELECT_SHORT:
							exit = true;

							break;
					}
				
					CONSTRAIN(miliseconds, 0, 999);	

					break;
				case STATE_SEC:
					switch (button_event) {
						case BUTTON_UP_SHORT:
							seconds += 1;

							break;
						case BUTTON_DOWN_SHORT:
							seconds -= 1;
											
							break;
						case BUTTON_LEFT_SHORT:
							next_state = STATE_MIN;

							break;
						case BUTTON_RIGHT_SHORT:
							next_state = STATE_MILI_SEC;

							break;
						case BUTTON_SELECT_SHORT:
							exit = true;

							break;
					}
				
					CONSTRAIN(seconds, 0, 60);

					break;
				case STATE_MIN:
					switch (button_event) {
						case BUTTON_UP_SHORT:
							minutes += 1;

							break;
						case BUTTON_DOWN_SHORT:
							minutes -= 1;
											
							break;
						case BUTTON_LEFT_SHORT:
							next_state = STATE_HOUR;

							break;
						case BUTTON_RIGHT_SHORT:
							next_state = STATE_SEC;

							break;
						case BUTTON_SELECT_SHORT:
							exit = true;

							break;
					}
				
					CONSTRAIN(minutes, 0, 60);

					break;
				case STATE_HOUR:
					switch (button_event) {
						case BUTTON_UP_SHORT:
							hours += 1;

							break;
						case BUTTON_DOWN_SHORT:
							hours -= 1;
											
							break;
						case BUTTON_LEFT_SHORT:
							next_state = STATE_MIN;

							break;
						case BUTTON_RIGHT_SHORT:
							next_state = STATE_DAY;

							break;
						case BUTTON_SELECT_SHORT:
							exit = true;

							break;
					}
				
					CONSTRAIN(hours, 0, 24);

					break;
				case STATE_DAY:
					switch (button_event) {
						case BUTTON_UP_SHORT:
							days += 1;

							break;
						case BUTTON_DOWN_SHORT:
							days -= 1;
											
							break;
						case BUTTON_RIGHT_SHORT:
							next_state = STATE_HOUR;

							break;
						case BUTTON_SELECT_SHORT:
							exit = true;

							break;
					}
				
					CONSTRAIN(days, 0, 999);

					break;
				default:
					next_state = STATE_SEC;
			}

			current_state = next_state;

		}
	}

	// Reset LCD Settings
}

//
// Menu Support Functions
//

void int2progress_bar(unsigned int value, unsigned int *progress) {
	unsigned int bars;

	bars = (value + 1) / 16;

	for (unsigned int x = 0; x < 16; x++) {
		if (x < bars) {
			progress[x] = 0xFF;
		} else {
			progress[x] = 0xFE;
		}
	}
}

// Send a menu to the LCD display
void print_menu(char *top_menu, char *bottom_menu) {
	// Clear the display and go home
	disp.clear_display();

	// Print splash screen
	disp.print(top_menu, 16);
	disp.set_display_address(LCD_LINE_2);
	disp.print(bottom_menu, 16);
}
