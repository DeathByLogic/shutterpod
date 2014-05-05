/*

	Project		: Shutter Pod
	File		: lcd.h
	Description	: LCD display class deffinition
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

#ifndef _LCD_H
#define _LCD_H

// Set the LCD interface bus width, either 8 or 4 bits
#define BUS_WIDTH		4

// Pins and ports for LCD interface
#define CMD_PORT		PORTB
#define	CMD_DIR			DDRB

#define RS_PIN			0x01
#define RW_PIN			0x02
#define	EN_PIN			0x04

// Pins and port for LCD data
#define DATA_WRITE_PORT	PORTC
#define DATA_READ_PORT	PINC
#define	DATA_DIR		DDRC

#if BUS_WIDTH == 4
	#define DATA_MASK	0x0F
	#define DATA_SHIFT	0
#endif

// Bit posistions for various commands / flags

// Masks for LCD commands
#define LCD_CLEAR_DISPLAY		0x01
#define LCD_CURSOR_HOME			0x02
#define LCD_ENTRY_MODE 			0x04
#define LCD_DISPLAY_CONTROL 	0x08
#define LCD_SHIFT_CONTROL	 	0x10
#define LCD_FUNCTION_SET		0x20
#define LCD_CGRAM_ADDRESS		0x40
#define LCD_DISPLAY_ADDRESS		0x80

#define LCD_ADDRESS_MASK		0x7F
#define LCD_BUSY_MASK			0x80
#define	LCD_RAM_MASK			0xFF

// Flags for LCD functions
#define	LCD_SHIFT_ON			0x01
#define LCD_SHIFT_OFF			0x00
#define LCD_INCREMENT			0x02
#define LCD_DECREMENT			0x00

#define LCD_CURSOR_BLINK_ON		0x01
#define LCD_CURSOR_BLINK_OFF	0x00
#define LCD_UNDERLINE_ON		0x02
#define LCD_UNDERLINE_OFF		0x00
#define LCD_ON					0x04
#define LCD_OFF					0x00

#define LCD_RIGHT_SHIFT			0x04
#define LCD_LEFT_SHIFT			0x00
#define LCD_DISPLAY_SHIFT		0x08
#define LCD_CURSOR_MOVE			0x00

#define LCD_5_BY_10				0x04
#define LCD_5_BY_7				0x00
#define LCD_2_LINE				0x08
#define LCD_1_LINE				0x00
#define	LCD_8_BIT_INTERFACE		0x10
#define	LCD_4_BIT_INTERFACE		0x00

// Misc constansts
#define LCD_LINE_1				0x00
#define LCD_LINE_2				0x40

#define LCD_DSP_LENGTH			16

class lcd {
	private:
		// Saved settings
		uint8_t cDisplay_flags;
		
		// Functions to send/receive data to display
		void send(bool, uint8_t);
		uint8_t receive(bool);
	public:
		// Constructor and destructor
		lcd(uint8_t flags);
		
		// Commands functions for LCD
		void clear_display();
		void display_home();
		void entry_mode(uint8_t flags);
		void display_control(uint8_t flags);
		void shift_control(uint8_t flags);
		void set_display_address(uint8_t address);
		void set_cgram_address(uint8_t address);
		
		// Display Options
		void display_on(bool value);
		void cursor_underline(bool value);
		void cursor_blink(bool value);
		
		// Print text string to LCD
		void print(char value);
		void print(int value);
		void print(char *string, uint8_t length);
		void print(uint8_t *array, uint8_t length);
		
		// Send raw commands / data to LCD
		void send_command(uint8_t parameters);
		void send_data(uint8_t value);
};

#endif
