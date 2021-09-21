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
 * -- $Revision: 1981 $
 * -- $Id: display.h 1981 2020-08-07 16:00:18Z ruan $
 * --------------------------------------------------------------- */
 
#ifndef __DISPLAY_H
#define __DISPLAY_H

/**
 * @brief  Initialize the display and write the initial content
 * 
 * @param  None
 * @retval None
 */
 void display_init(void);
 
/**
 * @brief  write user text and x, y and z values to display
 * @note   range of integer value: -9999 ... 9999
 * @param  x, y, z: 16 bit integer values
 * @retval None
 */
 void display_write_text_and_data(int16_t x, int16_t y, int16_t z);
 
/**
 * @brief  write value of who_am_I register to display
 * @param  reg_value: value to be displayed
 */
void display_write_who_am_I(uint8_t reg_value);

/**
 * see header file
 */
void display_write_text(uint8_t line_nr, const char* out_text);

#endif
