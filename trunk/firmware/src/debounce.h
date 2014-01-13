/*

	Project		: Shutter Pod
	File		: debounce.h
	Description	: Debounce class deffinition
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

#ifndef _DEBOUNCE_H
#define _DEBOUNCE_H

class debounce {
	private:
		int cb_short_flag;
		int cb_long_flag;
		void (*cb_function)(int flag, bool value);

		unsigned long debounce_counter;
		unsigned long repeat_counter;
		bool current_value;

	public:
		debounce(void (*call_back)(int flag, bool value), int short_flag, int long_flag);
		bool update(bool value);
};

#endif
