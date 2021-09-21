/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - Frequenzmessung
 * -- Description : Interface for lcd display
 * --
 * --
 * -- $Revision: 333 $
 * -- $Id: display_freq.h 333 2015-10-13 12:19:04Z ruan $
 * --------------------------------------------------------------- */


#ifndef _DISPLAY_FREQ
#define _DISPLAY_FREQ


#include <stdint.h>


/* type definitions ---------------------------------------------- */

/* macros for lcd */
#define RGB_CONTR_RED   (*((volatile uint16_t *)(0x60000340)))
#define RGB_CONTR_GREEN (*((volatile uint16_t *)(0x60000342)))
#define RGB_CONTR_BLUE  (*((volatile uint16_t *)(0x60000344)))
#define LCD_ASCII_CONTR (*((volatile uint16_t *)(0x60000310)))
#define LCD_ASCII_BSY   (*((volatile uint16_t *)(0x60000311)))


/* user defines */
#define CHARS_AT_ROW      40u
#define START_POS_SEC_ROW 40u


/* enum for display row */
typedef enum {
    DISP_ROW_ONE = 0x0,
    DISP_ROW_TWO = 0x1,
    DISP_ROW_BOTH = 0x2
}display_row;


/* function prototyp ---------------------------------------------- */

/**
 * \brief   Initialize lcd display with a background color and write
 *          the static text for frequence measurement to lcd.
 * \param   red : Background red, value between 0x0000 to 0xffff.
 * \param   green : Background green, value between 0x0000 to 0xffff.
 * \param   blue : Background blue, value between 0x0000 to 0xffff.
 */
void init_lcd_background_color(uint16_t red,
                               uint16_t green,
                               uint16_t blue);

/**
 * \brief   Convert binary value to bcd  value and write it on lcd.
 * \param   measured_freq: Binary frequency value.
 */
void write_tim3_freq_to_lcd(uint32_t measured_freq);

/**
 * \brief   Convert binary value to bcd  value and write it on lcd.
 * \param   measured_freq: Binary frequency value.
 */
void write_tim1_freq_to_lcd(uint32_t measured_freq);


/**
 *  \brief  Write a character to the pos on lcd display.
 *  \param  character : Char to write on lcd.
 *  \param  pos : Position on lcd.
 */
void write_char_to_lcd(char character, uint8_t pos);


/* end header ----------------------------------------------------- */
#endif
