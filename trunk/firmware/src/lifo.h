/*

	Project		: Shutter Pod
	File		: lifo.h
	Description	: LIFO class deffinition
	Date		: 1/6/2014

	Shutter Pod is an open source hardware timer for SLR cameras.
	Copyright (C) 2014 Daniel Wassenberg

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

struct node {
  struct node *next;

  int value;
};

class lifo {
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
