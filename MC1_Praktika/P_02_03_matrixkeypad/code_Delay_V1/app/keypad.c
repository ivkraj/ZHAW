/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab Matrix-Keypad
 * -- Description : Modul handles the reading of the keypad
 * --
 * -- $Id: keypad.c 1539 2018-09-20 06:17:18Z kjaz $
 * ------------------------------------------------------------------------- */

#include "keypad.h"
#include "hal_gpio.h"

/* -- Macros, type definitions and function prototypes
 * --------------------------------------------------------------------------*/
 
/// STUDENTS: To be programmed
#define ROW_MASK 0xF0
#define COLUMN_MASK 0x0F



/// END: To be programmed

/* internal variable definitions ------------------------------------------- */

static const int key_array [4][4] = { { 0x1, 0x2, 0x3, 0xF },
                                      { 0x4, 0x5, 0x6, 0xE },
                                      { 0x7, 0x8, 0x9, 0xD },
                                      { 0xA, 0x0, 0xB, 0xC } };


/* public function definitions --------------------------------------------- */
                                      
/*
 * See header file
 */
uint8_t scan_keypad_cbc(void)
{
    uint8_t ret_val = NOKEY_PRESSED;
    uint16_t buttons, row_values;
    /// STUDENTS: To be programmed
    
    hal_gpio_input_t row_cbc = { ROW_MASK, HAL_GPIO_PUPD_UP };
    hal_gpio_output_t column_cbc = { COLUMN_MASK, HAL_GPIO_PUPD_NOPULL, HAL_GPIO_OUT_SPEED_2MHZ, HAL_GPIO_OUT_TYPE_PP };
    hal_gpio_init_input(GPIOB, row_cbc);
    hal_gpio_init_output(GPIOB, column_cbc);
    
    for(uint8_t i = 0; i < 4; i++) {
        hal_gpio_output_write(GPIOB, COLUMN_MASK);
        hal_gpio_output_write(GPIOB, ~(0x01 << i));
        buttons = (hal_gpio_input_read(GPIOB) & ROW_MASK) >> 4;
        hal_gpio_output_write(GPIOB, COLUMN_MASK);
        
        row_values = ((~buttons) & 0xF);
        if (buttons != 0x0F) {
            switch(row_values){
                case 1:
                    row_values = 0;
                    break;
                case 2:
                    row_values = 1;
                    break;
                case 4:
                    row_values = 2;
                    break;
                case 8:
                    row_values = 3;
                    break; 
                default:
                    break;
            }
            ret_val = key_array[row_values][i]; 
            break;
        }
    }

    /// END: To be programmed
    return ret_val;
}

/*
 * See header file
 */
uint8_t scan_keypad_fast(void)
{
    uint8_t ret_val = NOKEY_PRESSED;
    /// STUDENTS: To be programmed
    uint16_t col_values, row_values, buttons;
    
    hal_gpio_input_t input_config = { ROW_MASK, HAL_GPIO_PUPD_UP };
    hal_gpio_output_t output_config = { COLUMN_MASK, HAL_GPIO_PUPD_NOPULL, HAL_GPIO_OUT_SPEED_2MHZ, HAL_GPIO_OUT_TYPE_PP };
    hal_gpio_init_input(GPIOB, input_config);
    hal_gpio_init_output(GPIOB, output_config);
    

    for(uint8_t i = 0; i < 4; i++) {
        hal_gpio_output_write(GPIOB, COLUMN_MASK);
        hal_gpio_output_write(GPIOB, ~(0x01 << i));
        buttons = (hal_gpio_input_read(GPIOB) & ROW_MASK) >> 4;
        hal_gpio_output_write(GPIOB, COLUMN_MASK);
        
        row_values = ((~buttons) & 0xF);
        if (buttons != 0x0F) {
            switch(row_values){
                case 1:
                    row_values = 0;
                    break;
                case 2:
                    row_values = 1;
                    break;
                case 4:
                    row_values = 2;
                    break;
                case 8:
                    row_values = 3;
                    break; 
                default:
                    break;
            }
            break;
        } else {
            row_values = 5;
        }
    }
    
    input_config.pins = COLUMN_MASK;
    output_config.pins = ROW_MASK;
    
    hal_gpio_init_input(GPIOB, input_config);
    hal_gpio_init_output(GPIOB, output_config);
    
    
    for(uint8_t i = 0; i < 4; i++) {
        hal_gpio_output_write(GPIOB, ROW_MASK);
        hal_gpio_output_write(GPIOB, ~(0x01 << (i+4)));
        buttons = (hal_gpio_input_read(GPIOB) & COLUMN_MASK);
        hal_gpio_output_write(GPIOB, ROW_MASK);
        
        col_values = ((~buttons) & 0xF);
        
        if (buttons != 0x0F) {
            switch(col_values){
                case 1:
                    col_values = 0;
                    break;
                case 2:
                    col_values = 1;
                    break;
                case 4:
                    col_values = 2;
                    break;
                case 8:
                    col_values = 3;
                    break; 
                default:
                    break;
            }
            break;
        }else {
            col_values = 5;
        }
    }
    
    if(row_values != 5 && col_values != 5){
        ret_val = key_array[row_values][col_values];
    } else{
        ret_val = NOKEY_PRESSED;
    }

    /// END: To be programmed
    return ret_val;
}

/* internal functions definitions ------------------------------------------ */
/// STUDENTS: To be programmed




/// END: To be programmed

