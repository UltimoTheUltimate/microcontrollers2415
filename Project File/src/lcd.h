/*!
 * \file      lcd.h
 * \brief     Driver for Hitachi HD44780 LCD controller in 4-bit mode.
 * \copyright ARM University Program &copy; ARM Ltd 2014.
 *
 * Modified for use with LPC4088 experiment bundle;
 * Copyright 2016-2017 Johann A. Briffa
 */
#ifndef LCD_H
#define LCD_H

/*! \brief Initialises the LCD module.
 */
void lcd_init(void);

/*! \brief Moves the cursor position to the specified location.
 *  \param column  Destination column for the cursor.
 *  \param row     Destination row for the cursor.
 */
void lcd_set_cursor(int column, int row);

/*! \brief Clears the LCD and relocates the cursor to {0,0}.
 */
void lcd_clear(void);

/*! \brief Prints the null terminated string to the LCD and
 *         increments the cursor.
 *  \param string  Null terminated string to be printed.
 */
void lcd_print(char *string);

/*! \brief Prints the specified character to the LCD and
 *         increments the cursor.
 *  \param c  Character to be printed.
 */
void lcd_put_char(char c);

/*! \brief Enables or disables visibility of the cursor.
 *  \param visible New visibility of the cursor.
 */
void lcd_set_cursor_visibile(int visible);

/*! \brief Scrolls a character on the top row of the LCD, shifting left when full.
 *  \param c  Character to be scrolled.
 */
void lcd_scroll_char(char c);

#endif // LDC_H
