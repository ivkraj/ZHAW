/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module stop_watch_ctrl.
 * 
 *  $Id: stop_watch_ctrl.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "stop_watch_ctrl.h"
#include "stop_watch.h"
#include "queue.h"
#include "lcd.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

//                           "                    "
#define TEXT_LINE_1         ("STOP WATCH    0>MODE")
#define TEXT_LINE_2_START   ("              1>GO  ")
#define TEXT_LINE_2_RESET   ("              1>RST ")
#define TEXT_LINE_2_STOP    ("              1>STOP")

/// STUDENTS: To be programmed

typedef enum {
    READY,
    RUN,
    STOP,
} sw_ctrl_state_t;

/* event queue for this FSM */
static queue_t stop_watch_ctrl_queue;

/* current state of the FSM */
static sw_ctrl_state_t state = READY;



void sw_ctrl_put_queue(stop_watch_ctrl_events_t event) {
    queue_enqueue(&stop_watch_ctrl_queue, event);

}

void sw_ctrl_handle_event(void)
{
    uint32_t event;
    
    event = queue_dequeue(&stop_watch_ctrl_queue);
    
    switch (state) {
        case READY:
            switch (event) {
                case SWC_BUTTON_EVENT:
                    state = RUN;
                    stop_watch_start();
                    break;
                case SWC_DISPLAY_UPDATE_EVENT:
                    lcd_output_update();
                    seg7_output_update();
                    break;
                default:
                    break;// no change
            }
            break;
        case RUN:
            switch (event) {
                case SWC_BUTTON_EVENT:
                    state = STOP;
                    stop_watch_stop();
                    break;
                case SWC_DISPLAY_UPDATE_EVENT:
                    lcd_output_update();
                    seg7_output_update();
                    break;
                default:
                    break;// no change
            }
            break;
        case STOP:
            switch (event) {
                case SWC_BUTTON_EVENT:
                    state = READY;
                    stop_watch_reset();
                    break;
                case SWC_DISPLAY_UPDATE_EVENT:
                    lcd_output_update();
                    seg7_output_update();
                    break;
                default:
                    break;// no change
            }
            break;
        default:
            ;// no change
    }
    
    /* Update outputs */
    if (event != SWC_NO_EVENT) {
        seg7_output_update();   
        lcd_output_update();        
    }
    
}


void sw_ctrl_update_display(void){
    lcd_write(LCD_LINE_1, TEXT_LINE_1);
    
    switch (state) {
        case READY:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_START);
            break;
        
        case RUN:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_STOP);
            break;
        
        case STOP:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_RESET);
            break;

    }
}


/// END: To be programmed
