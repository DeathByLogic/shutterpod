/*

	Project		: Shutter Pod
	File		: lcd.cpp
	Description	: LCD display class code
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
#include <avr/io.h> 
#include "lcd.h"

// Macros for LCD commands
#define SETBITS(x, y)	x |= y
#define CLEARBITS(x, y)	x &= ~y

// Class constructor and destructor
lcd::lcd(uint8_t flags) {
	SETBITS(CMD_DIR, RS_PIN | RW_PIN | EN_PIN);

#if BUS_WIDTH == 4
	send_command(LCD_FUNCTION_SET);
#else
	flags |= LCD_8_BIT_INTERFACE;
#endif

	send_command(LCD_FUNCTION_SET | flags);
}

//
// Public functions
//

// Command generation functions
void lcd::clear_display() {
	send_command(LCD_CLEAR_DISPLAY);
}

void lcd::display_home() {
	send_command(LCD_CURSOR_HOME);
}

void lcd::entry_mode(uint8_t flags) {
	send_command(LCD_ENTRY_MODE | flags);
}

void lcd::display_control(uint8_t flags) {
	cDisplay_flags = flags;

	send_command(LCD_DISPLAY_CONTROL | flags);
}

void lcd::shift_control(uint8_t flags) {
	send_command(LCD_SHIFT_CONTROL | flags);
}

void lcd::set_cgram_address(uint8_t address) {
	send_command(LCD_CGRAM_ADDRESS | address);
}

void lcd::set_display_address(uint8_t address) {
	send_command(LCD_DISPLAY_ADDRESS | address);
}

// Functions for specific commands
void lcd::display_on(bool value) {
	if (value == true) {
		SETBITS(cDisplay_flags, LCD_ON);
	} else {
		CLEARBITS(cDisplay_flags, LCD_ON);
	}

	send_command(LCD_DISPLAY_CONTROL | cDisplay_flags);
}

void lcd::cursor_underline(bool value) {
	if (value == true) {
		SETBITS(cDisplay_flags, LCD_UNDERLINE_ON);
	} else {
		CLEARBITS(cDisplay_flags, LCD_UNDERLINE_ON);
	}

	send_command(LCD_DISPLAY_CONTROL | cDisplay_flags);
}

void lcd::cursor_blink(bool value) {
	if (value == true) {
		SETBITS(cDisplay_flags, LCD_CURSOR_BLINK_ON);
	} else {
		CLEARBITS(cDisplay_flags, LCD_CURSOR_BLINK_ON);
	}

	send_command(LCD_DISPLAY_CONTROL | cDisplay_flags);
}

// Print text to screen
void lcd::print(char *string, uint8_t length) {
	for(uint8_t i = 0; i < length; i++) {
		send_data(string[i]);
	}
}

void lcd::print(uint8_t *array, uint8_t length) {
	for(uint8_t i = 0; i < length; i++) {
		send_data(array[i]);
	}
}

void lcd::print(char value) {
	send_data(value);
}

void lcd::print(int value) {
	send_data(value);
}

// Send functions
void lcd::send_command(uint8_t command) {
	while(receive(false) > 127) {
	//for(int i = 10000; i > 0; i--) {
		asm("NOP");
	}
	
	send(false, command);
}

void lcd::send_data(uint8_t value) {
	while(receive(false) > 127) {
	//for(int i = 10000; i > 0; i--) {
		asm("NOP");
	}

	send(true, value);
}

//
// Private functions
//

// Send data function
void lcd::send(bool RS, uint8_t command) {
	// Configure port as an output
#if BUS_WIDTH == 4
	SETBITS(DATA_DIR, DATA_MASK);
#else
	SETBITS(DATA_DIR, 0xFF);
#endif
	
	// Set or clear RS bit
	if (RS) {
		SETBITS(CMD_PORT, RS_PIN);
	} else {
		CLEARBITS(CMD_PORT, RS_PIN);
	}
	
	// Clear RW bit for write mode
	CLEARBITS(CMD_PORT, RW_PIN);
	
	// Send data to port
#if BUS_WIDTH == 4
	DATA_WRITE_PORT ^= ((command >> (4 - DATA_SHIFT)) ^ DATA_WRITE_PORT) & DATA_MASK;
#else
	DATA_WRITE_PORT = command;
#endif
	
	asm("NOP");
	
	// Set enable pin
	SETBITS(CMD_PORT, EN_PIN);
	
	asm("NOP");
	
	// Clear enable pin
	CLEARBITS(CMD_PORT, EN_PIN);

#if BUS_WIDTH == 4
	asm("NOP");

	DATA_WRITE_PORT ^= ((command << DATA_SHIFT) ^ DATA_WRITE_PORT) & DATA_MASK;
	
	asm("NOP");

	// Set enable pin
	SETBITS(CMD_PORT, EN_PIN);

	asm("NOP");

	// Clear enable pin
	CLEARBITS(CMD_PORT, EN_PIN);
#endif
}

// Read data function
uint8_t lcd::receive(bool RS) {
	int lcd_data = 0x00;
	
	// Configure port as in input
#if BUS_WIDTH == 4
	CLEARBITS(DATA_DIR, DATA_MASK);
	CLEARBITS(DATA_WRITE_PORT, DATA_MASK);
#else
	CLEARBITS(DATA_DIR, 0xFF);
	CLEARBITS(DATA_WRITE_PORT, 0xFF);
#endif	

	// Set or clear RS bit
	if (RS) {
		SETBITS(CMD_PORT, RS_PIN);
	} else {
		CLEARBITS(CMD_PORT, RS_PIN);
	}
	
	// Set RW bit for read mode
	SETBITS(CMD_PORT, RW_PIN);
	
	asm("NOP");
	
	// Set enable pin
	SETBITS(CMD_PORT, EN_PIN);
	
	for(unsigned int i = 3; i > 0; i--) {
		asm("NOP");
	}

#if BUS_WIDTH == 4
	lcd_data = (DATA_READ_PORT & DATA_MASK) << (4 - DATA_SHIFT);
#else
	lcd_data = DATA_READ_PORT;
#endif

	// Clear enable pin
	CLEARBITS(CMD_PORT, EN_PIN);

#if BUS_WIDTH == 4
	asm("NOP");

	// Set enable pin
	SETBITS(CMD_PORT, EN_PIN);
	
	for(unsigned int i = 3; i > 0; i--) {
		asm("NOP");
	}
	
	lcd_data |= (DATA_READ_PORT & DATA_MASK) >> DATA_SHIFT;
	
	// Clear enable pin
	CLEARBITS(CMD_PORT, EN_PIN);
#endif

	return lcd_data;
}


