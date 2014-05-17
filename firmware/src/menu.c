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

#include <avr/io.h>

#include "global.h"
#include "menu.h"
#include "camera.h"
#include "pwm.h"
#include "eeprom.h"
#include "low_power.h"

// Menu States
MENU_STATES menu_state = MENU_MAIN_SETTINGS;
TIME_STATES time_state = STATE_HUN_SEC;

//
// Menu Text
//

// Spash screen
char MSG_SPLASH_1[] =					"  Shutter  Pod  ";
char MSG_SPLASH_2[] =					"  Version  0.2  ";

// Main menu
char MSG_MAIN_MENU[] =					"Main Menu       ";

char MSG_SETTINGS_OPT[] =				"    Settings   >";
char MSG_TIME_LPS_OPT[] = 				"<  Time-lapse  >";
char MSG_LONG_EXP_OPT[] =				"<Long  Exposure>";
char MSG_MANNUAL_OPT[] =				"<    Manual    >";
char MSG_ABOUT_OPT[] =					"<    About      ";

// Settings menu
char MSG_SETTINGS_MENU[] =				"Settings        ";

char MSG_CAMERA_SETTINGS_OPT[] =		"     Camera    >";
char MSG_DISPLAY_SETTINGS_OPT[] =		"<    Display   >";

// Camera Settings menu
char MSG_CAMERA_MENU[] =				"Camera Settings ";

char MSG_CAMERA_FOCUS_OPT[] = 			"   Focus Time  >";
char MSG_CAMERA_DELAY_OPT[] = 			"<  Delay Time  >";
char MSG_CAMERA_SHUTTER_OPT[] = 		"< Shutter Time >";

// Display Settings menu
char MSG_DISPLAY_MENU[] =				"Display Settings";


char MSG_DISPLAY_BRIGHTNESS_OPT[] = 	"   Brightness  >";
char MSG_DISPLAY_TL_BRIGHTNESS_OPT[] =	"<TmLp Brghtness>";
char MSG_DISPLAY_TIMEOUT_OPT[] = 		"<   Time Out   >";
char MSG_DISPLAY_CONTRAST_OPT[] = 		"<   Contrast   >";

// Long Exposure menu
char MSG_LONGEXP_MENU[] =				"Long Exposure   ";

// Timelapse menu
char MSG_TIMELAPSE_MENU[] =         	"Timelapse       ";

// Manual menu
char MSG_MANUAL_MENU[] =            	"Manual          ";

// Generic menus
char MSG_START_OPT[] =					"     Start     >";
char MSG_TRIGGER_OPT[] =				"     Trigger   >";
char MSG_STOP_OPT[] =					"      Stop     >";
char MSG_CANCEL_OPT[] =					"     Cancel    >";

char MSG_SHTTER_TIME_OPT[] =			"< Shutter Time >";
char MSG_PERIOD_OPT[] =					"<    Period    >";
char MSG_DURATION_OPT[] =				"<   Duration   >";

char MSG_BACK_OPT[] =					"<     Back      ";

// Set Camera Settings
char MSG_CAMERA_FOCUS_TIME[] =			"Focus Time      ";
char MSG_CAMERA_DELAY_TIME[] =			"Shutter Delay   ";
char MSG_CAMERA_SHUTTER_TIME[] =		"Shutter Time    ";

// Set Display Settings
char MSG_DISPLAY_CONTRAST_LEVEL[] =		"Dsp Contrast    ";
char MSG_DISPLAY_TIMEOUT_PERIOD[] =		"Timeout Period  ";
char MSG_DISPLAY_BRIGHTNESS_LEVEL[] =   "Dsp Brightness  ";
char MSG_DISPLAY_TL_BRIGHTNESS_LEVEL[] = "TmLp Brightness ";

// Set Long Exposure Times
char MSG_LXP_SHUTTER_TIME[] =			"Lng Exp Shutter ";

// Set Timelapse Times
char MSG_TLP_PERIOD[] =					"Time Lapse Prd  ";
char MSG_TLP_DURATION[] =				"Time Lapse Dur  ";

//
// Functions
//

// System Menu FSM
void menu_fsm(uint8_t button_event) {
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
				case BUTTON_LEFT_SHORT:
					enter_pwr_dwn = true;
					
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
					menu_state = MENU_CAMERA_FOCUS_TIME;
					
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
					menu_state = MENU_DISPLAY_BRIGHTNESS;
					
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
		// Camera Settings
		//
		case MENU_CAMERA_FOCUS_TIME:
			switch (button_event) {
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_CAMERA_DELAY_TIME;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = SET_CAMERA_FOCUS_TIME;
					
					break;
			}

			break;
		case MENU_CAMERA_DELAY_TIME:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_CAMERA_FOCUS_TIME;
		
					break;
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_CAMERA_SHUTTER_TIME;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = SET_CAMERA_DELAY_TIME;
					
					break;
			}

			break;
		case MENU_CAMERA_SHUTTER_TIME:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_CAMERA_DELAY_TIME;
		
					break;
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_CAMERA_RETURN;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = SET_CAMERA_SHUTTER_TIME;
					
					break;
			}

			break;
		case MENU_CAMERA_RETURN:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_CAMERA_SHUTTER_TIME;
		
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = MENU_SETTINGS_CAMERA;
					
					break;
			}

			break;
		//
		// Display Settings
		//
		case MENU_DISPLAY_BRIGHTNESS:
			switch (button_event) {
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_DISPLAY_TL_BRIGHTNESS;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = SET_DISPLAY_BRIGHTNESS;
					
					break;
			}

			break;
		case MENU_DISPLAY_TL_BRIGHTNESS:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_DISPLAY_BRIGHTNESS;
		
					break;
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_DISPLAY_TIMEOUT;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Set timelapse brightness while adjusting
					set_backlight_dc(sys_param.le_brightness_level);

					menu_state = SET_DISPLAY_TL_BRIGHTNESS;
					
					break;
			}

			break;
		case MENU_DISPLAY_TIMEOUT:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_DISPLAY_TL_BRIGHTNESS;
		
					break;
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_DISPLAY_CONTRAST;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = SET_DISPLAY_TIMEOUT;
					
					break;
			}

			break;
		case MENU_DISPLAY_CONTRAST:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_DISPLAY_TIMEOUT;
		
					break;
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_DISPLAY_RETURN;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = SET_DISPLAY_CONTRAST;
					
					break;
			}

			break;
		case MENU_DISPLAY_RETURN:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_DISPLAY_CONTRAST;
		
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = MENU_SETTINGS_DISPLAY;
					
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
					menu_state = SET_LONGEXP_SHUTTER_TIME;
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
					menu_state = MENU_TIMELSP_PERIOD;
	
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
		case MENU_TIMELSP_PERIOD:
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
					menu_state = SET_TIMELSP_PERIOD;
					time_state = STATE_HUN_SEC;
					
					break;
			}

			break;
		case MENU_TIMELSP_DURATION:
			switch (button_event) {
				case BUTTON_LEFT_SHORT:
				case BUTTON_LEFT_LONG:
					menu_state = MENU_TIMELSP_PERIOD;
		
					break;
				case BUTTON_RIGHT_SHORT:
				case BUTTON_RIGHT_LONG:
					menu_state = MENU_TIMELSP_RETURN;
	
					break;
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					menu_state = SET_TIMELSP_DURATION;
					time_state = STATE_HUN_SEC;
					
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
		// Set Camera Times
		//
		case SET_CAMERA_FOCUS_TIME:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_CAMERA_FOCUS_TIME;

					break;
				default:
					get_time(&sys_param.focus_time, button_event);

					break;
			}

			break;
		case SET_CAMERA_DELAY_TIME:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_CAMERA_DELAY_TIME;

					break;
				default:
					get_time(&sys_param.shutter_delay, button_event);

					break;
			}

			break;
		case SET_CAMERA_SHUTTER_TIME:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_CAMERA_SHUTTER_TIME;

					break;
				default:
					get_time(&sys_param.shutter_time, button_event);

					break;
			}

			break;
		//
		// Set Display Setting
		//
		case SET_DISPLAY_CONTRAST:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_DISPLAY_CONTRAST;

					break;
				default:
					get_bar(&sys_param.contrast_level, button_event);
					set_contrast_dc(sys_param.contrast_level);

					break;
			}

			break;
		case SET_DISPLAY_TIMEOUT:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_DISPLAY_TIMEOUT;

					break;
				default:
					get_time(&sys_param.timeout_period, button_event);

					break;
			}

			break;
		case SET_DISPLAY_BRIGHTNESS:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_DISPLAY_BRIGHTNESS;

					break;
				default:
					get_bar(&sys_param.brightness_level, button_event);
					set_backlight_level();
				
					break;
			}

			break;
		case SET_DISPLAY_TL_BRIGHTNESS:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Reset back to normal brightness
					set_backlight_level();
					
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_DISPLAY_TL_BRIGHTNESS;

					break;
				default:
					get_bar(&sys_param.le_brightness_level, button_event);
					
					// Set timelapse brightness while adjusting
					set_backlight_dc(sys_param.le_brightness_level);
					break;
			}

			break;
		//
		// Set Long Exposure Times
		//
		case SET_LONGEXP_SHUTTER_TIME:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_LONGEXP_SHUTTER_TIME;

					break;
				default:
					get_time(&sys_param.le_shutter_time, button_event);

					break;
			}

			break;
		//
		// Set Timelapse Times
		//
		case SET_TIMELSP_PERIOD:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_TIMELSP_PERIOD;

					break;
				default:
					get_time(&sys_param.tl_period, button_event);

					break;
			}

			break;
		case SET_TIMELSP_DURATION:
			switch (button_event) {
				case BUTTON_SELECT_SHORT:
				case BUTTON_SELECT_LONG:
					// Update system param in eeprom
					update_sys_param();

					menu_state = MENU_TIMELSP_DURATION;

					break;
				default:
					get_time(&sys_param.tl_duration, button_event);

					break;
			}

			break;
	}
}

// Refresh the LCD display
void update_display(void) {
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
		// Camera Settings
		//
		case MENU_CAMERA_FOCUS_TIME:
			print_menu(MSG_CAMERA_MENU, MSG_CAMERA_FOCUS_OPT);

			break;
		case MENU_CAMERA_DELAY_TIME:
			print_menu(MSG_CAMERA_MENU, MSG_CAMERA_DELAY_OPT);

			break;
		case MENU_CAMERA_SHUTTER_TIME:
			print_menu(MSG_CAMERA_MENU, MSG_CAMERA_SHUTTER_OPT);

			break;
		case MENU_CAMERA_RETURN:
			print_menu(MSG_CAMERA_MENU, MSG_BACK_OPT);

			break;
		//
		// Display Settings
		//
		case MENU_DISPLAY_CONTRAST:
			print_menu(MSG_DISPLAY_MENU, MSG_DISPLAY_CONTRAST_OPT);

			break;
		case MENU_DISPLAY_TIMEOUT:
			print_menu(MSG_DISPLAY_MENU, MSG_DISPLAY_TIMEOUT_OPT);

			break;
		case MENU_DISPLAY_BRIGHTNESS:
			print_menu(MSG_DISPLAY_MENU, MSG_DISPLAY_BRIGHTNESS_OPT);

			break;
		case MENU_DISPLAY_TL_BRIGHTNESS:
			print_menu(MSG_DISPLAY_MENU, MSG_DISPLAY_TL_BRIGHTNESS_OPT);

			break;
		case MENU_DISPLAY_RETURN:
			print_menu(MSG_DISPLAY_MENU, MSG_BACK_OPT);

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
		case MENU_TIMELSP_PERIOD:
			print_menu(MSG_TIMELAPSE_MENU, MSG_PERIOD_OPT);

			break;
		case MENU_TIMELSP_DURATION:
			print_menu(MSG_TIMELAPSE_MENU, MSG_DURATION_OPT);

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
		// Set Camera Settings
		//
		case SET_CAMERA_FOCUS_TIME:
			print_menu(MSG_CAMERA_FOCUS_TIME, display_time(&sys_param.focus_time));

			break;
		case SET_CAMERA_DELAY_TIME:
			print_menu(MSG_CAMERA_DELAY_TIME, display_time(&sys_param.shutter_delay));

			break;
		case SET_CAMERA_SHUTTER_TIME:
			print_menu(MSG_CAMERA_SHUTTER_TIME, display_time(&sys_param.shutter_time));

			break;

		//
		// Set Display Settings
		//
		case SET_DISPLAY_CONTRAST:
			print_menu(MSG_DISPLAY_CONTRAST_LEVEL, display_bar(sys_param.contrast_level));

			break;
		case SET_DISPLAY_TIMEOUT:
			print_menu(MSG_DISPLAY_TIMEOUT_PERIOD, display_time(&sys_param.timeout_period));

			break;
		case SET_DISPLAY_BRIGHTNESS:
			print_menu(MSG_DISPLAY_BRIGHTNESS_LEVEL, display_bar(sys_param.brightness_level));

			break;
		case SET_DISPLAY_TL_BRIGHTNESS:
			print_menu(MSG_DISPLAY_TL_BRIGHTNESS_LEVEL, display_bar(sys_param.le_brightness_level));

			break;
		//
		// Set Long Exposure Times
		//
		case SET_LONGEXP_SHUTTER_TIME:
			print_menu(MSG_LXP_SHUTTER_TIME, display_time(&sys_param.le_shutter_time));

			break;
		//
		// Set Timelapse Times
		//
		case SET_TIMELSP_PERIOD:
			print_menu(MSG_TLP_PERIOD, display_time(&sys_param.tl_period));

			break;

		case SET_TIMELSP_DURATION:
			print_menu(MSG_TLP_DURATION, display_time(&sys_param.tl_duration));

			break;
	}
}

// Display the cursor if needed
void update_cursor(void) {
	if (menu_state == SET_DISPLAY_TIMEOUT or
		menu_state == SET_CAMERA_FOCUS_TIME or
		menu_state == SET_CAMERA_DELAY_TIME or
		menu_state == SET_CAMERA_SHUTTER_TIME or
		menu_state == SET_LONGEXP_SHUTTER_TIME or
		menu_state == SET_TIMELSP_PERIOD or
		menu_state == SET_TIMELSP_DURATION) {

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

// Send a menu to the LCD display
void print_menu(char *top_menu, char *bottom_menu) {
	// Clear the display and go home
	disp.clear_display();

	// Print splash screen
	disp.print(top_menu, 16);
	disp.set_display_address(LCD_LINE_2);
	disp.print(bottom_menu, 16);
}

// Generate text string of time value
char *display_time(unsigned long *time) {
	static char dsp_text[16];
	
	// Calculate individual parts of time
	uint8_t hun_sec = get_hun_sec(time);
	uint8_t sec = get_sec(time);
	uint8_t min = get_min(time);
	uint8_t hour = get_hour(time);
	uint8_t day = get_day(time);
	
	// Generate text of time to display
	dsp_text[0] = ' ';
	dsp_text[1] = TO_ASCII(day / 100);
	dsp_text[2] = TO_ASCII((day % 100) / 10);
	dsp_text[3] = TO_ASCII(day % 10);
	dsp_text[4] = ' ';
	dsp_text[5] = TO_ASCII(hour / 10);
	dsp_text[6] = TO_ASCII(hour % 10);
	dsp_text[7] = ':';
	dsp_text[8] = TO_ASCII(min / 10);
	dsp_text[9] = TO_ASCII(min % 10);
	dsp_text[10] = ':';
	dsp_text[11] = TO_ASCII(sec / 10);
	dsp_text[12] = TO_ASCII(sec % 10);
	dsp_text[13] = '.';
	dsp_text[14] = TO_ASCII(hun_sec / 10);
	dsp_text[15] = TO_ASCII(hun_sec % 10);

	return dsp_text;
}

// Generate a bar graph string
char *display_bar(uint8_t value) {
	static char dsp_text[16];
	uint8_t bars;

	bars = (value + 1) / 16;

	for (uint8_t x = 0; x < 16; x++) {
		if (x < bars) {
			dsp_text[x] = 0xFF;
		} else {
			dsp_text[x] = 0xFE;
		}
	}

	return dsp_text;
}

//
// Generate human reable data for display
//

void get_time(unsigned long *time, uint8_t button_event) {
	switch (time_state) {
		case STATE_HUN_SEC:
			switch (button_event) {
				case BUTTON_UP_SHORT:
					*time += 1;

					break;
				case BUTTON_UP_LONG:
					*time += 10;

					break;
				case BUTTON_DOWN_SHORT:
					*time -= 1;
									
					break;
				case BUTTON_DOWN_LONG:
					*time -= 10;
									
					break;
				case BUTTON_LEFT_SHORT:
					time_state = STATE_SEC;

					break;
			}

			break;
		case STATE_SEC:
			switch (button_event) {
				case BUTTON_UP_SHORT:
					*time += SEC_TICK;

					break;
				case BUTTON_UP_LONG:
					*time += 10 * SEC_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					*time -= SEC_TICK;
									
					break;
				case BUTTON_DOWN_LONG:
					*time -= 10 * SEC_TICK;
									
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
					*time += MIN_TICK;

					break;
				case BUTTON_UP_LONG:
					*time += 10 * MIN_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					*time -= MIN_TICK;
									
					break;
				case BUTTON_DOWN_LONG:
					*time -= 10 * MIN_TICK;
									
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
					*time += HOUR_TICK;

					break;
				case BUTTON_UP_LONG:
					*time += 10 * HOUR_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					*time -= HOUR_TICK;
									
					break;
				case BUTTON_DOWN_LONG:
					*time -= 10 * HOUR_TICK;
									
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
					*time += DAY_TICK;

					break;
				case BUTTON_UP_LONG:
					*time += 10 * DAY_TICK;

					break;
				case BUTTON_DOWN_SHORT:
					*time -= DAY_TICK;
									
					break;
				case BUTTON_DOWN_LONG:
					*time -= 10 * DAY_TICK;
									
					break;
				case BUTTON_RIGHT_SHORT:
					time_state = STATE_HOUR;

					break;
			}

			break;
		default:
			time_state = STATE_HUN_SEC;
	}
}

void get_bar(uint8_t *value, uint8_t button_event) {
	switch (button_event) {
		case BUTTON_UP_SHORT:
		case BUTTON_UP_LONG:
		case BUTTON_RIGHT_SHORT:
		case BUTTON_RIGHT_LONG:
			*value = CONSTRAIN(*value + 16, 0, 255);

			break;
		case BUTTON_DOWN_SHORT:
		case BUTTON_DOWN_LONG:
		case BUTTON_LEFT_SHORT:
		case BUTTON_LEFT_LONG:
			*value = CONSTRAIN(*value - 16, 0, 255);

			break;
	}
}

//
// Get human readable time
//

uint8_t get_hun_sec(unsigned long *time) {
	return (uint8_t)(*time % SEC_TICK);
}

uint8_t get_sec(unsigned long *time) {
	return (uint8_t)((*time % MIN_TICK) / SEC_TICK);
}

uint8_t get_min(unsigned long *time) {
	return (uint8_t)((*time % HOUR_TICK) / MIN_TICK);
}

uint8_t get_hour(unsigned long *time) {
	return (uint8_t)((*time % DAY_TICK) / HOUR_TICK);
}

uint8_t get_day(unsigned long *time) {
	return (uint8_t)(*time / DAY_TICK);
}
