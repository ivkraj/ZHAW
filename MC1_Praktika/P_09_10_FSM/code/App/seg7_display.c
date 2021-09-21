/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module scheduler.
 * 
 *  $Id: seg7_display.c 1302 2017-11-22 10:50:03Z kjaz $
 * ------------------------------------------------------------------------- */

 
/* User includes */
#include "reg_ctboard.h"
#include "seg7_display.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define NULL                ((void *)0)
#define NR_OF_DIGITS        4u
#define NEXT_DIGIT          3u
#define NEXT_VALUE          4u
#define SEGMENT_DOT         0x80

/* -- Variables with module-wide scope
 * ------------------------------------------------------------------------- */
 
static seg7_callback_t get_output_callback = NULL;


/* -- Local function declarations
 * ------------------------------------------------------------------------- */

static uint8_t seg7_convert_to_7seg(uint8_t value);


/* -- Public function definitions
 * ------------------------------------------------------------------------- */
 
/*
 *  See header file
 */
void seg7_output_update(void)
{
    seg7_output_t output = {0, 0};
    uint32_t digit = 0u;
    uint8_t i;
    
    if (get_output_callback != NULL) {
        get_output_callback(&output);
        
        for (i = 0u; i < NR_OF_DIGITS; i++) {
			// write each digit (DS3-0) with the values containing in output.value
            digit |= seg7_convert_to_7seg((uint8_t) output.value) << (i << NEXT_DIGIT);
            output.value >>= NEXT_VALUE;
			// set the dot depending on output.dot
            if ((output.dots >> i) & 0x01) {
                digit &= ~(SEGMENT_DOT << (i << NEXT_DIGIT));
            }
        } 
        
        CT_7SEG->RAW.WORD = digit;
    }
}


/*
 *  See header file
 */
void seg7_register_get_output_callback(seg7_callback_t function)
{
    get_output_callback = function;
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Convert 4 bit value into 7 segment code.
 *  \param  value : Value to be converted.
 *  \return Converted 7 segment code.
 */
static uint8_t seg7_convert_to_7seg(uint8_t value)
{
    static const uint8_t conv_tab[16u] = { 0xc0, 0xf9, 0xa4, 0xb0,
                                           0x99, 0x92, 0x82, 0xf8,
                                           0x80, 0x90, 0x88, 0x83,
                                           0xa7, 0xa1, 0x86, 0x8e };
    
    return conv_tab[value & 0x0f];
}
