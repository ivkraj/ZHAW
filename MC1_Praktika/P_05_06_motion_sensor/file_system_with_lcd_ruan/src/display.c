/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - energy efficient sensors
 * -- Description : This modul handles the interface to the Display
 * --               on the Discovery board
 * --               
 * --               Normally, the lcd uses a buffer in the SDRAM of 
 * --               the discovery board. However, as the filesystem
 * --               board uses the SDRAM pins on the ports P5, P6 and P7,
 * --               this applicaton has to use internal SRAM.
 * --               Unfortunately, the internal SRAM is too small to
 * --               allocate a large enough buffer.
 * --               This module uses enough space to make lines 0 to 5
 * --               available and leaving the rest of the lcd dark.
 * --               To make more lines availble, one would have to 
 * --               either increase the frame_buffer size or alternatively
 * --               to lower the resolution (instead of ARGB8888)
 * -- 
 * -- $Revision: 2021 $
 * -- $Id: display.c 2021 2020-09-23 05:46:16Z ruan $
 * --------------------------------------------------------------- */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "display.h"
#include "stm32f429i_discovery_lcd.h"

// as large as possible in internal SRAM. 76'800 also seems to work with some
// restrictions
//static uint16_t frame_buffer[96000];
static uint16_t frame_buffer[76800];

#define DISPLAY_STRING_LENGTH 15

/**
 * see header file
 */
void display_init(void)
{
    BSP_LCD_Init();
    
    // Typically, one would use the pre-defined LCD_FRAME_BUFFER in SDRAM
    // but we use internal SRAM for the reasons explained in the file header
    BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, (uint32_t)&frame_buffer);
    BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
    
    // the DMA in BSP_LCD_Clear() overwrites global variable LcdDrv
    //   --> crashes into Hardfault_Handler()
    // BSP_LCD_Clear(LCD_COLOR_WHITE);      // Pixel Format: ARGB8888
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    display_write_text_and_data(1000, -1000, 1000);
}

/**
 * see header file
 */
void display_write_text_and_data(int16_t x, int16_t y, int16_t z)
{
    // Caveat: Writing to line 6 as well as 8 or higher creates hard faults
    //         Interestingly, line 7 works
    // Reason for hardfaults: Missing memory in internal SRAM
    
    char text[DISPLAY_STRING_LENGTH] = " Acceleration ";

    BSP_LCD_DisplayStringAtLine(1, (uint8_t *)text);
    
    ( void )snprintf(text, DISPLAY_STRING_LENGTH, "  x:  %5dmg ", x);
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *)text);

    ( void )snprintf(text, DISPLAY_STRING_LENGTH, "  y:  %5dmg ", y);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)text);

    ( void )snprintf(text, DISPLAY_STRING_LENGTH, "  z:  %5dmg ", z);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)text);
}

/**
 * see header file
 */
void display_write_who_am_I(uint8_t reg_value)
{
    char text[DISPLAY_STRING_LENGTH];

    ( void )snprintf(text, DISPLAY_STRING_LENGTH, " who: 0x%x    ", reg_value);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t *)text);
}

/**
 * see header file
 */
void display_write_text(uint8_t line_nr, const char* out_text)
{
    char text[DISPLAY_STRING_LENGTH];
    
    // only use upper half of display due to memory limitations
    if (line_nr < 6) {
        (void)strncpy(text, out_text, DISPLAY_STRING_LENGTH);
        BSP_LCD_DisplayStringAtLine(line_nr, (uint8_t *)text);
    }
}
