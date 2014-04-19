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

#include <math.h>
#include <avr/io.h>

#include "global.h"
#include "lcd.h"
#include "fifo.h"
#include "menu.h"
#include "misc.h"
#include "camera.h"
#include "screen_text.h"

// External variables
extern lcd disp;
extern fifo button_events;

// Menu States
MENU_STATES menu_state = MENU_MAIN_SETTINGS;
TIME_STATES time_state = STATE_HUN_SEC;

// dispay the splash menu
void display_splash() {
	// Clear the display and go home
	disp.clear_display();

	// Print splash screen
	disp.print(MSG_SPLASH_1, 16);
	disp.set_display_address(LCD_LINE_2);
	disp.print(MSG_SPLASH_2, 16);
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

//
// System Menu FSM
//

void menu_main(void) {
	// Pull next button event off stack
	int button_event = button_events.pop();

	if (button_event > 0) {
		switch (menu_state) {
			//
			// Main Menu
			//
			case MENU_MAIN_SETTINGS:
				switch (button_event) {
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_MAIN_LONGEXP;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_SETTINGS_CAMERA;
						
						break;
				}

				break;
			case MENU_MAIN_LONGEXP:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_MAIN_SETTINGS;
			
						break;
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_MAIN_TIMELSP;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_LONGEXP_START_CANCEL;
						break;
				}

				break;
			case MENU_MAIN_TIMELSP:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_MAIN_LONGEXP;
			
						break;
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_MAIN_MANUAL;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_TIMELSP_START_STOP;
						
						break;
				}

				break;
			case MENU_MAIN_MANUAL:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_MAIN_TIMELSP;
			
						break;
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_MAIN_ABOUT;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_MANUAL_TRIGGER;
						
						break;
				}

				break;
			case MENU_MAIN_ABOUT:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_MAIN_MANUAL;
			
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						//menu_state = ;
						
						break;
				}

				break;
			//
			// Settings Menu
			//
			case MENU_SETTINGS_CAMERA:
				switch (button_event) {
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_SETTINGS_DISPLAY;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						//menu_state = ;
						
						break;
				}

				break;
			case MENU_SETTINGS_DISPLAY:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_SETTINGS_CAMERA;
			
						break;
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_SETTINGS_RETURN;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						//menu_state = ;
						
						break;
				}

				break;
			case MENU_SETTINGS_RETURN:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_SETTINGS_DISPLAY;
			
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_MAIN_SETTINGS;
						
						break;
				}

				break;
			//
			// Long Exposure Menu
			//
			case MENU_LONGEXP_START_CANCEL:
				switch (button_event) {
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_LONGEXP_SHUTTER_TIME;
			
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						if (camera_mode == MODE_LONGEXP) {
							camera_mode = MODE_IDLE;
						} else {
							camera_mode = MODE_LONGEXP;
						}
						
						
						break;
				}

				break;
			case MENU_LONGEXP_SHUTTER_TIME:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_LONGEXP_START_CANCEL;
			
						break;
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_LONGEXP_RETURN;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = SET_LXP_SHUTTER_TIME;
						time_state = STATE_HUN_SEC;
						
						break;
				}

				break;
			case MENU_LONGEXP_RETURN:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_LONGEXP_SHUTTER_TIME;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_MAIN_LONGEXP;
						
						break;
				}

				break;
			//
			// Time Lapse Menu
			//
			case MENU_TIMELSP_START_STOP:
				switch (button_event) {
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_TIMELSP_FREQ;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						if (camera_mode == MODE_TIMELAPSE) {
							camera_mode = MODE_IDLE;
						} else {
							camera_mode = MODE_TIMELAPSE;
						}
						
						break;
				}

				break;
			case MENU_TIMELSP_FREQ:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_TIMELSP_START_STOP;
			
						break;
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_TIMELSP_DURATION;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						//menu_state = ;
						
						break;
				}

				break;
			case MENU_TIMELSP_DURATION:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_TIMELSP_FREQ;
			
						break;
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_TIMELSP_RETURN;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						//menu_state = ;
						
						break;
				}

				break;
			case MENU_TIMELSP_RETURN:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_TIMELSP_DURATION;
			
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_MAIN_TIMELSP;
						
						break;
				}

				break;
			//
			// Manual Menu
			//
			case MENU_MANUAL_TRIGGER:
				switch (button_event) {
					case BUTTON_RIGHT_SHORT:
					case BUTTON_RIGHT_LONG:
						menu_state = MENU_MANUAL_RETURN;
			
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						//menu_state = ;
						
						break;
				}

				break;
			case MENU_MANUAL_RETURN:
				switch (button_event) {
					case BUTTON_LEFT_SHORT:
					case BUTTON_LEFT_LONG:
						menu_state = MENU_MANUAL_TRIGGER;
		
						break;
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_MAIN_MANUAL;
						
						break;
				}

				break;
			//
			// Set Times
			//
			case SET_LXP_SHUTTER_TIME:
				switch (button_event) {
					case BUTTON_SELECT_SHORT:
					case BUTTON_SELECT_LONG:
						menu_state = MENU_LONGEXP_SHUTTER_TIME;

						break;
					default:
						le_shutter_time = get_time(le_shutter_time, button_event);

						break;
				}
		}

	display_menu();
	}
}

void display_menu(void) {
	char time_text[16];

	switch (menu_state) {
		//
		// Main Menu
		//
		case MENU_MAIN_SETTINGS:
			print_menu(MSG_MAIN_MENU, MSG_SETTINGS_OPT);

			break;
		case MENU_MAIN_LONGEXP:
			print_menu(MSG_MAIN_MENU, MSG_LONG_EXP_OPT);

			break;
		case MENU_MAIN_TIMELSP:
			print_menu(MSG_MAIN_MENU, MSG_TIME_LPS_OPT);

			break;
		case MENU_MAIN_MANUAL:
			print_menu(MSG_MAIN_MENU, MSG_MANNUAL_OPT);

			break;
		case MENU_MAIN_ABOUT:
			print_menu(MSG_MAIN_MENU, MSG_ABOUT_OPT);

			break;
		//
		// Settings Menu
		//
		case MENU_SETTINGS_CAMERA:
			print_menu(MSG_SETTINGS_MENU, MSG_CAMERA_SETTINGS_OPT);

			break;
		case MENU_SETTINGS_DISPLAY:
			print_menu(MSG_SETTINGS_MENU, MSG_DISPLAY_SETTINGS_OPT);

			break;
		case MENU_SETTINGS_RETURN:
			print_menu(MSG_SETTINGS_MENU, MSG_BACK_OPT);

			break;
		//
		// Long Exposure
		//
		case MENU_LONGEXP_START_CANCEL:
			if (camera_mode == MODE_LONGEXP) {
				print_menu(MSG_LONGEXP_MENU, MSG_CANCEL_OPT);
			} else {
				print_menu(MSG_LONGEXP_MENU, MSG_START_OPT);
			}

			break;
		case MENU_LONGEXP_SHUTTER_TIME:
			print_menu(MSG_LONGEXP_MENU, MSG_SHTTER_TIME_OPT);

			break;
		case MENU_LONGEXP_RETURN:
			print_menu(MSG_LONGEXP_MENU, MSG_BACK_OPT);

			break;
		//
		// Time-lapse
		//
		case MENU_TIMELSP_START_STOP:
			if (camera_mode == MODE_TIMELAPSE) {
				print_menu(MSG_TIMELAPSE_MENU, MSG_STOP_OPT);
			} else {
				print_menu(MSG_TIMELAPSE_MENU, MSG_START_OPT);
			}

			break;
		case MENU_TIMELSP_FREQ:
			print_menu(MSG_TIMELAPSE_MENU, MSG_FREQ_OPT);

			break;
		case MENU_TIMELSP_DURATION:
			print_menu(MSG_TIMELAPSE_MENU, MSG_PERIOD_OPT);

			break;
		case MENU_TIMELSP_RETURN:
			print_menu(MSG_TIMELAPSE_MENU, MSG_BACK_OPT);

			break;
		//
		// Manual Menu
		//
		case MENU_MANUAL_TRIGGER:
			print_menu(MSG_MANUAL_MENU, MSG_TRIGGER_OPT);

			break;
		case MENU_MANUAL_RETURN:
			print_menu(MSG_MANUAL_MENU, MSG_BACK_OPT);

			break;
		//
		// Set Times
		//
		case SET_LXP_SHUTTER_TIME:
			print_menu(MSG_LXP_SHUTTER_TIME, display_time(le_shutter_time));

			break;
	}

	// Update the current cursor posistion
	display_cursor();
}

char *display_time(unsigned long time) {
	static char dsp_text[16];

	int hun_sec = get_hun_sec(time);
	int sec = get_sec(time);
	int min = get_min(time);
	int hour = get_hour(time);
	int day = get_day(time);

	for (int i = 0; i < 16; i++) {
		switch (i) {
			case 1:
				dsp_text[i] = TO_ASCII(day / 100);
				break;
			case 2:
				dsp_text[i] = TO_ASCII((day % 100) / 10);
				break;
			case 3:
				dsp_text[i] = TO_ASCII(day % 10);
				break;
			case 5:
				dsp_text[i] = TO_ASCII(hour / 10);
				break;
			case 6:
				dsp_text[i] = TO_ASCII(hour % 10);
				break;
			case 7:
				dsp_text[i] = ':';
				break;
			case 8:
				dsp_text[i] = TO_ASCII(min / 10);
				break;
			case 9:
				dsp_text[i] = TO_ASCII(min % 10);
				break;
			case 10:
				dsp_text[i] = ':';
				break;
			case 11:
				dsp_text[i] = TO_ASCII(sec / 10);
				break;
			case 12:
				dsp_text[i] = TO_ASCII(sec % 10);
				break;
			case 13:
				dsp_text[i] = '.';
				break;
			case 14:
				dsp_text[i] = TO_ASCII(hun_sec / 10);
				break;
			case 15:
				dsp_text[i] = TO_ASCII(hun_sec % 10);
				break;
			default:
				dsp_text[i] = ' ';
				break;
		}
	}

	return dsp_text;
}

unsigned long get_time(unsigned long time, int button_event) {
	switch (time_state) {
		case STATE_HUN_SEC:
			switch (button_event) {
				case BUTTON_UP_SHORT:
					time += 1;

					break;
				case BUTTON_DOWN_SHORT:
					time -= 1;
									
					break;
				case BUTTON_LEFT_SHORT:
					time_state = STATE_SEC;

					break;
			}

			break;
		case STATE_SEC:
			switch (button_event) {
				case BUTTON_UP_SHORT:
					time += SEC_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					time -= SEC_TICK;
									
					break;
				case BUTTON_LEFT_SHORT:
					time_state = STATE_MIN;

					break;
				case BUTTON_RIGHT_SHORT:
					time_state = STATE_HUN_SEC;

					break;
			}

			break;
		case STATE_MIN:
			switch (button_event) {
				case BUTTON_UP_SHORT:
					time += MIN_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					time -= MIN_TICK;
									
					break;
				case BUTTON_LEFT_SHORT:
					time_state = STATE_HOUR;

					break;
				case BUTTON_RIGHT_SHORT:
					time_state = STATE_SEC;

					break;
			}

			break;
		case STATE_HOUR:
			switch (button_event) {
				case BUTTON_UP_SHORT:
					time += HOUR_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					time -= HOUR_TICK;
									
					break;
				case BUTTON_LEFT_SHORT:
					time_state = STATE_DAY;

					break;
				case BUTTON_RIGHT_SHORT:
					time_state = STATE_MIN;

					break;
			}

			break;
		case STATE_DAY:
			switch (button_event) {
				case BUTTON_UP_SHORT:
					time += DAY_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					time -= DAY_TICK;
									
					break;
				case BUTTON_RIGHT_SHORT:
					time_state = STATE_HOUR;

					break;
			}

			break;
		default:
			time_state = STATE_HUN_SEC;
	}

	// Reset LCD Settings
	return time;
}

void display_cursor(void) {
	if (menu_state == SET_LXP_SHUTTER_TIME) {
		// Configure LCD Settings
		disp.cursor_underline(true);

		switch (time_state) {
			case STATE_HUN_SEC:
				disp.set_display_address(0x4F);

				break;
			case STATE_SEC:
				disp.set_display_address(0x4C);

				break;
			case STATE_MIN:
				disp.set_display_address(0x49);

				break;
			case STATE_HOUR:
				disp.set_display_address(0x46);

				break;
			case STATE_DAY:
				disp.set_display_address(0x43);

				break;
		}
	} else {
		// Configure LCD Settings
		disp.cursor_underline(false);
	}
}

//
// Menu Support Functions
//
/*
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
*/

