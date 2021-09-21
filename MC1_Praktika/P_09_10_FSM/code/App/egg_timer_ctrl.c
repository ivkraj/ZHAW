/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module egg_timer_ctrl.
 * 
 *  $Id: egg_timer_ctrl.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "egg_timer_ctrl.h"
#include "egg_timer.h"
#include "queue.h"
#include "lcd.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

//                           "                    "
#define TEXT_LINE_1         ("EGG TIMER     0>MODE")
#define TEXT_LINE_2_START   ("              1>GO  ")
#define TEXT_LINE_2_RESET   ("              1>RST ")
#define TEXT_LINE_2_STOP    ("              1>STOP")


/* -- Local type definitions
 * ------------------------------------------------------------------------- */

/* enumerate the states */
typedef enum {
    STOPPED,
    RUN,
    ALARM
} egg_timer_ctrl_state_t;


/* -- Variables with module-wide scope
 * ------------------------------------------------------------------------- */

/* event queue for this FSM */
static queue_t et_ctrl_queue;

/* current state of the FSM */
static egg_timer_ctrl_state_t state = STOPPED;


/* -- Local function declarations
 * ------------------------------------------------------------------------- */

static void et_ctrl_called_on_elapsed(void);


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/**
 *  see header file
 */
void et_ctrl_put_queue(egg_timer_ctrl_events_t event)
{
    queue_enqueue(&et_ctrl_queue, event);
}


/*
 *  See header file
 */
void et_ctrl_handle_event(void)
{
    uint32_t event;

    event = queue_dequeue(&et_ctrl_queue);

    switch (state) {
        case STOPPED:
            switch (event) {
                case ETC_BUTTON_EVENT:
                    state = RUN;
                    egg_timer_start(et_ctrl_called_on_elapsed);
                    lcd_color_set(LCD_GREEN);
                    break;

                default:
                    ;// no change
            }
            break;

        case RUN:
            switch (event) {
                case ETC_BUTTON_EVENT:
                    state = STOPPED;
                    egg_timer_stop();
                    lcd_color_set(LCD_OFF);
                    break;
                case ETC_ELAPSED_EVENT:
                    state = ALARM;
                    egg_timer_stop();
                    lcd_color_set(LCD_RED);
                    break;
                default:
                    ;// no change
            }
            break;

        case ALARM:
            switch (event) {
                case ETC_BUTTON_EVENT:
                    state = STOPPED;
                    lcd_color_set(LCD_OFF);
                    break;

                default:
                    ;// no change
            }
            break;
        default:
            ;// no change
    }
    
    /* Update outputs */
    if (event != ETC_NO_EVENT) {
        seg7_output_update();   
        lcd_output_update();        
    }
}


/*
 *  See header file
 */
void et_ctrl_update_display(void)
{
    lcd_write(LCD_LINE_1, TEXT_LINE_1);
    
    switch (state) {
        case STOPPED:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_START);
            break;
        
        case RUN:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_STOP);
            break;
        
        case ALARM:
            lcd_write(LCD_LINE_2, TEXT_LINE_2_RESET);
            break;
    }
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/**
 *  \brief  This function is registered as a call-back in the egg_timer
 *          It places an elapsed event in the queue of the egg_timer_ctrl FSM
 */
static void et_ctrl_called_on_elapsed(void)
{
    et_ctrl_put_queue(ETC_ELAPSED_EVENT);
}
