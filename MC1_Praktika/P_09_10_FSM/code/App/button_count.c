/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module button_count.
 * 
 *  $Id: button_count.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "button_count.h"
#include "queue.h"
#include "lcd.h"
#include "reg_ctboard.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

//                           "                    "
#define TEXT_LINE_1         ("BUTTONS       0>MODE")
#define TEXT_LINE_2_T0      ("              1>T0  ")
#define TEXT_LINE_2_T1      ("              1>T1  ")
#define TEXT_LINE_2_T2      ("              1>T2  ")
#define TEXT_LINE_2_T3      ("              1>T3  ")

/// STUDENTS: To be programmed

typedef enum {
    DISP_COUNT_T0,
    DISP_COUNT_T1,
    DISP_COUNT_T2,
    DISP_COUNT_T3
} button_count_state_t;

/* event queue for this FSM */
static queue_t button_count_queue;

/* current state of the FSM */
static button_count_state_t state = DISP_COUNT_T0;

typedef struct {
    uint16_t counter_t0;
    uint16_t counter_t1;
    uint16_t counter_t2;
    uint16_t counter_t3;
} button_counter_t;

static button_counter_t button_counter = {0, 0, 0, 0};

static void button_counter_enqueue_button (uint32_t action){
    switch (action){
        case T0_PRESSED:
            button_counter.counter_t0++;
            break;
        case T1_PRESSED:
            button_counter.counter_t1++;
            break;
        case T2_PRESSED:
            button_counter.counter_t2++;
            break;
        case T3_PRESSED:
            button_counter.counter_t3++;
            break;
        default:
            break;
    }       
}

void button_count_put_queue(button_count_events_t event) {
    queue_enqueue(&button_count_queue, event);
}

void button_count_init(void){
    buttons_register_observer(button_counter_enqueue_button);
}

void button_count_handle_event(void)
{
    uint32_t event;
    
    event = queue_dequeue(&button_count_queue);
    
    switch (state) {
        case DISP_COUNT_T0:
            switch (event) {
                case BCC_BUTTON_T1_EVENT:
                    state = DISP_COUNT_T1;
                    break;
                default:
                    break;// no change
            }
            break;
        case DISP_COUNT_T1:
            switch (event) {
                case BCC_BUTTON_T1_EVENT:
                    state = DISP_COUNT_T2;
                    break;
                default:
                    break;// no change
            }
            break;
        case DISP_COUNT_T2:
            switch (event) {
                case BCC_BUTTON_T1_EVENT:
                    state = DISP_COUNT_T3;
                    break;
                default:
                    break;// no change
            }
            break;
        case DISP_COUNT_T3:
            switch (event) {
                case BCC_BUTTON_T1_EVENT:
                    state = DISP_COUNT_T0;
                    break;
                default:
                    break;// no change
            }
            break;
        default:
            break;// no change
    }
    
    seg7_output_update();   
    lcd_output_update();        
    
}

void button_count_update_display(void){
    lcd_write(LCD_LINE_1, TEXT_LINE_1);
    
    switch (state) {
        case DISP_COUNT_T0:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_T1);
            break;
        
        case DISP_COUNT_T1:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_T2);
            break;
        
        case DISP_COUNT_T2:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_T3);
            break;
        
        case DISP_COUNT_T3:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_T0);
            break;

    }
}

uint16_t buttons_count_get_bcd(button_counter_t *button_count, button_count_state_t state)
{
    uint16_t temp = 0;
    uint16_t bcd = 0;
    uint8_t numb = 0;
        
    /* Convert seconds */
    switch (state){
        case DISP_COUNT_T0:
            temp = button_count->counter_t0;
            numb = 0;
            break;
        case DISP_COUNT_T1:
            temp = button_count->counter_t1;
            numb = 1;    
            break;
        case DISP_COUNT_T2:
            temp = button_count->counter_t2;
            numb = 2;
            break;
        case DISP_COUNT_T3:
            temp = button_count->counter_t3;
            numb = 3;
            break;
    }
    
    bcd = (temp%10);
    temp /= 10;
    bcd |= (temp%10) << 4;
    temp /= 10;
    bcd |= (temp%10) << 8;
    bcd |= numb << 12u;
    
    return bcd;
}

void button_count_get_output(seg7_output_t *output){
    
    output->value = buttons_count_get_bcd(&button_counter, state);
    output->dots = (0x1 << 3u);     // Set dot on DS2

}


/// END: To be programmed
