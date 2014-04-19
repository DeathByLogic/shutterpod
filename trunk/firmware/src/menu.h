/*

	Project		: Shutter Pod
	File		: menu.h
	Description	: 
	Date		: 3/20/2011

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

#ifndef _MENU_H
#define _MENU_H

#define TO_ASCII(x) x + 0x30

// Menu State Declorations
enum MENU_STATES {
	MENU_MAIN_SETTINGS,
	MENU_MAIN_LONGEXP,
	MENU_MAIN_TIMELSP,
	MENU_MAIN_MANUAL,
	MENU_MAIN_ABOUT,

	MENU_SETTINGS_CAMERA,
	MENU_SETTINGS_DISPLAY,
	MENU_SETTINGS_RETURN,

	MENU_LONGEXP_START_CANCEL,
	MENU_LONGEXP_SHUTTER_TIME,
	MENU_LONGEXP_RETURN,

	MENU_TIMELSP_START_STOP,
	MENU_TIMELSP_FREQ,
	MENU_TIMELSP_DURATION,
	MENU_TIMELSP_RETURN,

	MENU_MANUAL_TRIGGER,
	MENU_MANUAL_RETURN,

	SET_LXP_SHUTTER_TIME
};

enum TIME_STATES {
	STATE_HUN_SEC,
	STATE_SEC,
	STATE_MIN,
	STATE_HOUR,
	STATE_DAY
};

// Function constructs
void display_splash(void);
void display_menu(void);
void display_cursor(void);
void menu_main(void);

unsigned long get_time(unsigned long, int);

void print_menu(char *, char *);
char *display_time(unsigned long);

// External global variables
/*
class menu_type {
	public:
		menu_type(char * menu_text, char * item_text, menu_type * up, menu_type * down, menu_type * left, menu_type * right);

		char * m_txt;
		char * i_txt;

		class menu_type * n_up;
		class menu_type * n_down;
		class menu_type * n_left;
		class menu_type * n_right;
}*/

// Misc

#endif
