/*

	Project		: Shutter Pod
	File		: debounce.cpp
	Description	: Debounce class code
	Date		: 4/6/2011

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
#include "debounce.h"

// Defines
#define DEBOUNCE_COUNT	78

debounce::debounce(void (*call_back)(int flag, bool value), int flag) {
	cb_flag = flag;
	cb_function = call_back;
}

bool debounce::update(bool value) {
	// Debounce button inputs
	if (current_value == value) {
		// Clear counter
		counter = 0;
	} else {
		// Increment counter
		counter++;
		
		// Check if debounce period has passed
		if (counter > DEBOUNCE_COUNT) {
			// Update current value
			current_value = value;

			// Call callback function
			cb_function(cb_flag, current_value);
		}
	}

	return current_value;
}
