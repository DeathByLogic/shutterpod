/*

	Project		: Shutter Pod
	File		: screen_text.h
	Description	: Text strings for displaying on LCD
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

#ifndef _SCREEN_TEXT_H
#define _SCREEN_TEXT_H

// Spash screen
char MSG_SPLASH_1[] =				"  Shutter  Pod  ";
char MSG_SPLASH_2[] =				"  Version  0.1  ";

// Main menu
char MSG_MAIN_MENU[] =				"Main Menu       ";

char MSG_SETTINGS_OPT[] =			"    Settings  ->";
char MSG_TIME_LPS_OPT[] = 			"<- Time-lapse ->";
char MSG_LONG_EXP_OPT[] =			"<- Long Expos ->";
char MSG_MANNUAL_OPT[] =			"<-   Manual   ->";
char MSG_ABOUT_OPT[] =              "<-   About      ";

// Settings menu
char MSG_SETTINGS_MENU[] =			"Settings        ";

char MSG_CAMERA_SETTINGS_OPT[] =	"     Camera   ->";
char MSG_DISPLAY_SETTINGS_OPT[] =	"<-   Display  ->";

// Long Exposure menu

char MSG_LONGEXP_MENU[] =           "Long Exposure   ";

// Timelapse menu
char MSG_TIMELAPSE_MENU[] =         "Timelapse       ";

// Manual menu
char MSG_MANUAL_MENU[] =            "Manual          ";

// Generic menus
char MSG_START_OPT[] =              "     Start    ->";
char MSG_TRIGGER_OPT[] =            "     Trigger  ->";
char MSG_STOP_OPT[] =               "      Stop    ->";
char MSG_CANCEL_OPT[] =             "     Cancel   ->";

char MSG_SHTTER_TIME_OPT[] =        "<-Shutter Time->";
char MSG_FREQ_OPT[] =               "<- Delay Time ->";
char MSG_PERIOD_OPT[] =             "<-   Period   ->";

char MSG_BACK_OPT[] =				"<-    Back      ";

// Set Times
char MSG_LXP_SHUTTER_TIME[] =       "Lng Exp Shutter ";

#endif
