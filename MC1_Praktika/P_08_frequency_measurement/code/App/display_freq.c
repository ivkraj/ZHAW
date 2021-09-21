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
 * -- Description : Implementation of module display.
 * --               Init the lcd display and write the measured
 * --               frequence on the lcd display.
 * --
 * --
 * -- $Revision: 945 $
 * -- $Id: display_freq.c 945 2016-10-17 13:14:05Z feur $
 * --------------------------------------------------------------- */


/* user includes */
#include <stdint.h>
#include "display_freq.h"
#include "hal_ct_lcd.h"

/* user defines */
#define WAIT_TIME 0x3fff

/* local var for module */
static uint32_t old_value;

/* prototypes for internal functions*/
static int binary_to_bcd_char_array(uint32_t binary,
                                    char *bcd_char_array_ret,
                                    uint8_t size_of_array);

static void reset_lcd_display_row(display_row row);
                                    

                                    
//////////////////////////////////////////////////////////////////////////////////////////////
                                    
void write_tim3_freq_to_lcd(uint32_t measured_freq)
{
    uint8_t i = 0, digs = 1;
    char integer_char[36];
    char si_unit[] = "Hz";

    /* nothing has changed */
    if (old_value == measured_freq) {
        return;
    }

    reset_lcd_display_row(DISP_ROW_TWO);

    digs += binary_to_bcd_char_array(measured_freq, integer_char,
                                     sizeof(integer_char));

    if (digs > 7) {
        integer_char[sizeof(integer_char) - 8] = '.';
        write_char_to_lcd('M', 37);
    } else if (digs > 3) {
        integer_char[sizeof(integer_char) - 4] = '.';
        write_char_to_lcd('k', 37);
    }

    for (i = 35; i > 35 - digs; i--) {
        write_char_to_lcd(integer_char[i], i);
    }

    digs = sizeof(si_unit) - 1;
    for (i = 76; i > 76 - sizeof(si_unit); i--) {
        write_char_to_lcd(si_unit[digs], i - 36);
        digs--;
//        if (i < START_POS_SEC_ROW) {
//            break;
//        }
    }
}
     //////////////////////////////////////////////////////////////////                               

/*
 * See header file
 */
void write_tim1_freq_to_lcd(uint32_t measured_freq)
{
    uint8_t i = 0, digs = 1;
    char integer_char[16];
    char si_unit[] = "Hz";

    /* nothing has changed */
    if (old_value == measured_freq) {
        return;
    }

    reset_lcd_display_row(DISP_ROW_ONE);

    digs += binary_to_bcd_char_array(measured_freq, integer_char,
                                     sizeof(integer_char));

    if (digs > 7) {
        integer_char[sizeof(integer_char) - 8] = '.';
        write_char_to_lcd('M', 17);
    } else if (digs > 3) {
        integer_char[sizeof(integer_char) - 4] = '.';
        write_char_to_lcd('k', 17);
    }

    for (i = 15; i > 15 - digs; i--) {
        write_char_to_lcd(integer_char[i], i);
    }

    digs = sizeof(si_unit) - 1;
    for (i = 56; i > 56 - sizeof(si_unit); i--) {
        write_char_to_lcd(si_unit[digs], i - 36);
        digs--;
        if (i < START_POS_SEC_ROW) {
            break;
        }
    }
}


/*
 * See header file
 */
void init_lcd_background_color(uint16_t red, uint16_t green, uint16_t blue)
{
    char freq [] = "f MEAS:             f REF:";                 // static text on first row
    uint8_t pos = 0;

    /* set backlight */
    RGB_CONTR_BLUE = blue;
    RGB_CONTR_GREEN = green;
    RGB_CONTR_RED = red;

    old_value = 0;

    hal_ct_lcd_clear();

    /* write static text to lcd */
    for (pos = 0; pos < sizeof(freq) / sizeof(char); pos++) {
        write_char_to_lcd(freq[pos], pos + 1);
        if (pos >= CHARS_AT_ROW) {
            break;
        }
    }
}


/**
 *  \brief  Write a character to the pos on lcd display.
 *  \param  character : Char to write on lcd.
 *  \param  pos : Position on lcd.
 */
void write_char_to_lcd(char character, uint8_t pos)
{
    uint16_t i;
    for (i = WAIT_TIME; i <= 0; i--);
    hal_ct_lcd_write(pos, &character);
    for (i = WAIT_TIME; i <= 0; i--);
}


/**
 *  \brief  Reset the chars on the lcd display.
 *  \param  row : Row to reset.
 */
static void reset_lcd_display_row(display_row row)
{
    switch (row) {
        case DISP_ROW_ONE:
            hal_ct_lcd_write(9u, "          ");
            break;
        case DISP_ROW_TWO:
            hal_ct_lcd_write(29u, "          ");
            break;
        default:
            hal_ct_lcd_clear();
            break;
    }
}


/**
 *  \brief  Calculate form a value the bcd number
            and returs it in a char array.
 *          Every 3 digs (1000er) is insert a space.
 *          Eg. int 123456789 to char array {1,2,3,.,4,5,6, ,7,8,9}
 *  \param  binary  : The value to convert.
 *  \param  bcd_char_array_ret :
            Pointer to char array, who save the bcd value (return).
 *  \param  size_of_array : Size of char array bcd_char_array_ret.
 *  \return Nummber of digs in the char array.
 */
static int binary_to_bcd_char_array(uint32_t binary,
                                    char *bcd_char_array_ret,
                                    uint8_t size_of_array)
{
    int i = 0, tenner = 0, counter = 0;
    for (i = size_of_array - 1; i >= 0; i--) {
        if (tenner == 3) {
            bcd_char_array_ret[i] = ' ';
            tenner = 0;
            i--;
            counter++;
        }
        bcd_char_array_ret[i] = binary % 10 + 48;
        binary = binary / 10;
        if (binary == 0) {
            break;
        }
        tenner++;
        counter++;
    }
    return counter;
}
