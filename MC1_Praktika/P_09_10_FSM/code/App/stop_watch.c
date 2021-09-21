/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module stop_watch.
 * 
 *  $Id: stop_watch.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "stop_watch.h"
#include "scheduler.h"

/// STUDENTS: To be programmed


/* -- Variables with module-wide scope
 * ------------------------------------------------------------------------- */

static scheduler_task_t task_increment;

typedef struct {
    uint16_t seconds;
    uint8_t hundred_milliseconds;
} stop_watch_time_t;

static stop_watch_time_t stop_watch_timer = { 0u, 0u };



/* -- Local function declarations
 * ------------------------------------------------------------------------- */

static void stop_watch_timer_increment(void){
    if (stop_watch_timer.hundred_milliseconds == 9){
        stop_watch_timer.seconds ++;
        stop_watch_timer.hundred_milliseconds = 0;
    } else {
        stop_watch_timer.hundred_milliseconds++;
    }
    if (stop_watch_timer.seconds == 1000) {
        stop_watch_timer.seconds = 0;
    }
    seg7_output_update();
    
}

uint16_t sw_time_get_bcd(stop_watch_time_t *time)
{
    uint16_t temp;
    uint16_t bcd;
        
    /* Convert seconds */
    bcd = time->hundred_milliseconds;
    
    /* Convert minutes */
    temp = time->seconds;
    bcd |= (temp%10) << 4;
    temp /= 10u;
    bcd |= (temp % 10u) << 8u;
    
    temp /= 10u;
    bcd |= (temp % 10u) << 12u;
    
    return bcd;
}

void stop_watch_get_output(seg7_output_t *output)
{
    output->value = sw_time_get_bcd(&stop_watch_timer);
    output->dots = (0x1 << 1u);     // Set dot on DS2
}


void stop_watch_start(void){
    seg7_output_update();
    scheduler_register_task(&task_increment, stop_watch_timer_increment, HUNDRED_MILLISECONDS);
}

void stop_watch_stop(void){
    scheduler_unregister_task(&task_increment);
}


void stop_watch_reset(void){
    stop_watch_timer.seconds = 0;
    stop_watch_timer.hundred_milliseconds = 0;
}


/// END: To be programmed
