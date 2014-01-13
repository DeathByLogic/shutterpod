/*

	Project		: Shutter Pod
	File		: fifo.h
	Description	: FIFO class deffinition
	Date		: 3/21/2011

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

#ifndef _FIFO_H
#define _FIFO_H

struct node {
  struct node *next;

  int value;
};

class fifo {
	private:
		// Location of first and last nodes
		node *first;
		node *last;

		node nodes[16];

		// Housekeeping
		bool is_full;
		bool is_empty;

	public:
		// Initilization
		fifo(void);

		// Pop and Push functions
		int pop(void);
		void push(int value);

		// Full and Empty functions
		bool empty(void);
		bool full(void);
};

#endif
