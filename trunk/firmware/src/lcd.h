/*  
 *  File	: lcd.h
 *  Date	: 6/23/2010
 *
 *  Project	: Shutter Pod
 *  Author	: D. Wassenberg
 *  Purpose	: Class constructs for LCD interface
 *
 */

// Macros for LCD commands
#define SETBITS(x, y)	x |= y
#define CLEARBITS(x, y)	x &= ~y

// Set the LCD interface bus width, either 8 or 4 bits
#define BUS_WIDTH		4

// Pins and ports for LCD interface
#define CMD_PORT		PORTB
#define	CMD_DIR			DDRB

#define RS_PIN			0x01
#define RW_PIN			0x02
#define	EN_PIN			0x04

// Pins and port for LCD data
#define DATA_WRITE_PORT	PORTD
#define DATA_READ_PORT	PIND
#define	DATA_DIR		DDRD

#if BUS_WIDTH == 4
	#define DATA_MASK	0xF0
	#define DATA_SHIFT	4
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
		unsigned int cDisplay_flags;
		
		// Functions to send/receive data to display
		void send(bool, unsigned int);
		unsigned int receive(bool);
	public:
		// Constructor and destructor
		lcd(unsigned int flags);
		
		// Commands functions for LCD
		void clear_display();
		void display_home();
		void entry_mode(unsigned int flags);
		void display_control(unsigned int flags);
		void shift_control(unsigned int flags);
		void set_display_address(unsigned int address);
		void set_cgram_address(unsigned int address);
		
		// Display Options
		void display_on(bool value);
		void cursor_underline(bool value);
		void cursor_blink(bool value);
		
		// Print text string to LCD
		void print(char *string, unsigned int length);
		void print(unsigned int *array, unsigned int length);
		
		// Send raw commands / data to LCD
		void send_command(unsigned int parameters);
		void send_data(unsigned int value);
};
