/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module buttons.
 * 
 *  $Id: lcd.c 1071 2016-11-18 12:38:50Z ruan $
 * ------------------------------------------------------------------------- */


#include "lcd.h"
#include "hal_ct_lcd.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define NULL                ((void *)0)


/* -- Variables with module-wide scope
 * ------------------------------------------------------------------------- */
 
static lcd_callback_t get_output_callback = NULL;


/* -- Public function definitions
 * ------------------------------------------------------------------------- */
 
/*
 *  See header file
 */
void lcd_output_update(void)
{
    if (get_output_callback != NULL) {
        get_output_callback();
    }
}


/*
 *  See header file
 */
void lcd_register_get_output_callback(lcd_callback_t function)
{
    get_output_callback = function;
}


/**
 * see header file
 */
void lcd_color_set(lcd_color_t color)
{
    /* Clear old color */
    hal_ct_lcd_color(HAL_LCD_RED, 0x0000);
    hal_ct_lcd_color(HAL_LCD_GREEN, 0x0000);
    hal_ct_lcd_color(HAL_LCD_BLUE, 0x0000);
    
    /* Set new color */
    switch (color) {
        case LCD_RED:
            hal_ct_lcd_color(HAL_LCD_RED, 0xffff);
            break;
        case LCD_GREEN:
            hal_ct_lcd_color(HAL_LCD_GREEN, 0xffff);
            break;
        case LCD_BLUE:
            hal_ct_lcd_color(HAL_LCD_BLUE, 0xffff);
            break;
        default:
            break;
    }
}

/**
 * see header file
 */
void lcd_write(uint8_t position, char text[])
{    
    hal_ct_lcd_write(position, text);
}
