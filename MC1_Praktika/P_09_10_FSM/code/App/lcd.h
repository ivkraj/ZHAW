/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module buttons.
 *
 *  $Id: lcd.h 1071 2016-11-18 12:38:50Z ruan $
 * ------------------------------------------------------------------------- */
 
/* Re-definition guard */
#ifndef _LCD_H
#define _LCD_H

/* Standard includes */
#include <stdint.h>


/* -- Macros
 * ------------------------------------------------------------------------- */
 
#define LCD_LINE_1  (0u)
#define LCD_LINE_2  (20u)


/* -- Type definitions
 * ------------------------------------------------------------------------- */

/**
 *  \struct lcd_text_t
 *  \brief  Enumerates the actions.
 */
typedef enum {
    RELOAD_CTRL,
    RELOAD_S,
    RELOAD_M,
    STOP_WATCH,
    EGG_TIMER
} lcd_text_t;

/**
 *  \struct lcd_color_t
 *  \brief  Enumerates the colors of the LCD.
 */
typedef enum {
    LCD_RED,
    LCD_GREEN,
    LCD_BLUE,
    LCD_OFF
} lcd_color_t;

/**
 *  \brief  Declaration of callback signature.
 */
typedef void (*lcd_callback_t)(void);


/* -- Public function declarations
 * ------------------------------------------------------------------------- */
 
/**
 *  \brief  Output data to 7 segment display.
 *
 *  If you call this function the value retrieved by the callback function
 *  gets displayed on the 7 segment display.
 */
void lcd_output_update(void);

/**
 *  \brief  Register callback for LCD.
 *
 *  The callback function to register shall have the following signature:
 *      void function_name(lcd_output_t *output);
 *
 *  The output argument is a pointer to a lcd_output_t structure, which
 *  contains the text for each zone for outputting on the display.
 *  The callback function has to fill the struct with data.
 *
 *  \param  function : Callback function to register.
 */
void lcd_register_get_output_callback(lcd_callback_t function);

/**
 *  \brief  Writes the specified string to the specified zone on the lcd.
 *          LCD_ZONE_TITLE -> max. 13 chars
 *          LCD_ZONE_Tx    -> max.  6 chars
 *  \param  zone : Specifies the zone in which the text shall be displayed.
 *  \param  text : Specifies which text shall be displayed.
 */
void lcd_write(uint8_t position, char text[]);

/**
 *  \brief  Sets the background color of the lcd
 *  \param  color   Desired background color, use the values defined
 *                  in the enum
 */
void lcd_color_set(lcd_color_t color);
#endif
