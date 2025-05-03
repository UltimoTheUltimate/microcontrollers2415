#include <platform.h>
#include <stdint.h>
#include <gpio.h>
#include "lcd.h"
#include "delay.h"

/* Modified for use with LPC4088 experiment bundle;
 * Copyright 2016-2017 Johann A. Briffa
 */

// Pin definitions for serial to parallel converter
#define PIN_SER  P1_24
#define PIN_SCK  P1_20
#define PIN_RCK  P1_2

// Mapping between serial port expander pins and LCD controller
#define D_LCD_PIN_D4   0
#define D_LCD_PIN_D5   1
#define D_LCD_PIN_D6   2
#define D_LCD_PIN_D7   3
#define D_LCD_PIN_RS   4
#define D_LCD_PIN_E    5

// Bitpattern definitions for above
#define D_LCD_D4       (1<<D_LCD_PIN_D4)
#define D_LCD_D5       (1<<D_LCD_PIN_D5)
#define D_LCD_D6       (1<<D_LCD_PIN_D6)
#define D_LCD_D7       (1<<D_LCD_PIN_D7)
#define D_LCD_RS       (1<<D_LCD_PIN_RS)
#define D_LCD_E        (1<<D_LCD_PIN_E)

// *** Internal functions - SPI handler ***

// Internal bus mirror value for serial bus only
uint8_t _spi_bus;

// Low level writes to LCD serial bus only (serial expander)
void spi_writeBus() {
	uint8_t c = _spi_bus;
	int i;
	for (i = 0; i < 8; i++) {
		// shift data MSB first.
		gpio_set(PIN_SER, !!(c & 0x80));
		// clock data into shift register.
		delay_us(1);
		gpio_set(PIN_SCK, 1);
		delay_us(1);
		gpio_set(PIN_SCK, 0);
		// shift the next bit
		c = c << 1;
	}
	// clock data to output latches.
	delay_us(1);
	gpio_set(PIN_RCK, 1);
	delay_us(1);
	gpio_set(PIN_RCK, 0);
}

// Initialization
void spi_init(void) {
	// set the relevant pins as output
	gpio_set_mode(PIN_SER, Output);
	gpio_set_mode(PIN_SCK, Output);
	gpio_set_mode(PIN_RCK, Output);
	// set all pins to low
	gpio_set(PIN_SER, 0);
	gpio_set(PIN_SCK, 0);
	gpio_set(PIN_RCK, 0);

	// Init the portexpander bus
	_spi_bus = 0;

	// write the new data to the portexpander
	spi_writeBus();
}

// Set E pin
void lcd_setEnable(char value) {

	if (value) {
		_spi_bus |= D_LCD_E;     // Set E bit
	}
	else {
		_spi_bus &= ~D_LCD_E;    // Reset E bit
	}

  // write the new data to the SPI portexpander
  spi_writeBus();
}

// Set RS pin
void lcd_setRS(char value) {

  if (value) {
    _spi_bus |= D_LCD_RS;    // Set RS bit
  }
  else {
    _spi_bus &= ~D_LCD_RS;   // Reset RS bit
  }

  // write the new data to the SPI portexpander
  spi_writeBus();
}

// Place the 4bit data on the databus
void lcd_setData(int value) {
  int data = value & 0x0F;

  // Set bit by bit to support any mapping of expander portpins to LCD pins

  if (data & 0x01) {
    _spi_bus |= D_LCD_D4;   // Set Databit
  }
  else {
    _spi_bus &= ~D_LCD_D4;  // Reset Databit
  }

  if (data & 0x02) {
    _spi_bus |= D_LCD_D5;   // Set Databit
  }
  else {
    _spi_bus &= ~D_LCD_D5;  // Reset Databit
  }

  if (data & 0x04) {
    _spi_bus |= D_LCD_D6;   // Set Databit
  }
  else {
    _spi_bus &= ~D_LCD_D6;  // Reset Databit
  }

  if (data & 0x08) {
    _spi_bus |= D_LCD_D7;   // Set Databit
  }
  else {
    _spi_bus &= ~D_LCD_D7;  // Reset Databit
  }

  // write the new data to the SPI portexpander
  spi_writeBus();
}


// *** Internal functions - controller interface ***

void lcd_write_4bit(uint8_t c) {
	lcd_setEnable(1);
	lcd_setData(c & 0x0F);
	delay_us(1);
	lcd_setEnable(0);
	delay_us(1);
}

static void lcd_write_data(uint8_t c) {
	lcd_setRS(1);
	lcd_write_4bit(c>>4);
	lcd_write_4bit(c);
}

void lcd_write_cmd(uint8_t c) {
	lcd_setRS(0);
	lcd_write_4bit(c>>4);
	lcd_write_4bit(c);
}

// *** Exported functions ***

// Initialises the LCD module.
void lcd_init(void) {
	// Set up serial-parallel interface
	spi_init();

	// Run LCD initilisation sequence
	lcd_setRS(0);
	lcd_write_4bit(0x3);
	delay_us(4100);
	lcd_write_4bit(0x3);
	delay_us(100);
	lcd_write_4bit(0x3);
	lcd_write_4bit(0x2);
	lcd_write_cmd(0x28); // Function set.
	lcd_write_cmd(0x0C);
	lcd_write_cmd(0x06);
	lcd_set_cursor(0, 0);
}

// Enables or disables visibility of the cursor.
void lcd_set_cursor_visibile(int visible) {
	lcd_write_cmd(0x0C | ((!!visible) << 1));
}

// Moves the cursor position to the specified location.
void lcd_set_cursor(int column, int row) {
	int address;
	address = (row * 0x40) + column;
	address |= 0x80;
	lcd_write_cmd(address);
}

// Clears the LCD and relocates the cursor to {0,0}.
void lcd_clear(void) {
	lcd_write_cmd(0x01);
	delay_us(1520);
}

// Prints the specified character to the LCD and increments the cursor.
void lcd_put_char(char c) {
	lcd_write_data(c);
}

// Prints the null terminated string to the LCD and increments the cursor.
void lcd_print(char *string) {
	while(*string) {
		lcd_put_char(*string++);
	}
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************
