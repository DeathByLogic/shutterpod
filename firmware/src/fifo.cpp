/*

	Project		: Shutter Pod
	File		: fifo.cpp
	Description	: FIFO class code
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

// Includes
#include "fifo.h"
#include "misc.h"

// Class constructor
fifo::fifo(void) {
	is_empty = true;
	is_full = false;

	// Link nodes into loop
	for (int i = 0; i < 16; i++) {
		nodes[i].next = &nodes[(i + 1) % 16];
	}
	
	// Set first and last link
	first = &nodes[0];
	last = &nodes[15];
}

//
// Public functions
//

// Pop a value off the FIFO
int fifo::pop(void) {
	if (!is_empty) {
		// Save the node value
		int value = first->value;
		
		if (first == last) {
			is_empty = true;
		} else {
			// Move to next node
			first = first->next;
		}

		// Check if full
		is_full = (last->next == first);

		return value;
	} else {
		return 0;
	}
}

// Push a new value onto FIFO
void fifo::push(int value) {
	node *new_node;

	if (!is_full) {
		if (is_empty) {
			new_node = last;
		} else {
			// Create a new node
			new_node = last->next;
		}

		// Store the value in the new node
		new_node->value = value;

		// save the new end node
		last = new_node;
		
		// No longer empty
		is_empty = false;

		// Check if full
		is_full = (last->next == first);
	}
}

// Housekeeping funtions
bool fifo::empty(void) {
	return is_empty;
}

bool fifo::full(void) {
	return is_full;
}
