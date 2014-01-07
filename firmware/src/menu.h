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

//
// Menu State Declorations
//

enum {
	MENU_MAIN_SETTINGS,
	MENU_MAIN_LONGEXP,
	MENU_MAIN_TIMELSP,
	MENU_MAIN_MANUAL,
	MENU_SETTINGS_CAMERA,
	MENU_SETTINGS_DISPLAY,
	MENU_LONGEXP,
	MENU_TIMELSP,
	MENU_MANUAL
};

/*
// Main Menu
#define MENU_MAIN			0x00

#define MENU_MAIN_SETTINGS	0x01
#define MENU_MAIN_LONGEXP	0x02
#define MENU_MAIN_TIMELSP	0x04
#define MENU_MAIN_MANUAL	0x08

// Settings Menu
#define MENU_SETTINGS		0x10

#define MENU_SETTINGS_CAMERA	0x11
#define MENU_SETTINGS_DISPLAY	0x12
#define MENU_SETTINGS_RETURN	0x14

// Long Exposure Menu
#define MENU_LONGEXP		0x20

// Time Lapse Menu
#define MENU_TIMELSP		0x40

// Manual Mode Menu
#define MENU_MANUAL			0x80*/

// Function constructs
void display_splash(void);
void menu_main(void);

int menu_main_longexp(int button_event);
int menu_main_timelsp(int button_event);
int menu_main_manual(int button_event);
int menu_main_settings(int button_event);

void print_menu(char *, char *);

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
