/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module egg_timer.
 * 
 *  $Id: egg_timer.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

 
/* User includes */
#include "egg_timer.h"
#include "reload.h"
#include "scheduler.h"
#include "time.h"


/* -- Variables with module-wide scope
 * ------------------------------------------------------------------------- */

static scheduler_task_t task_decrement;
static time_t egg_timer = { 0u, 0u };


/* -- Local function declarations
 * ------------------------------------------------------------------------- */

static void (*elapsed_call_back)(void);
static void egg_timer_decrement(void);


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void egg_timer_start(void (*func_ptr)(void))
{
    egg_timer = reload_get_time();
    seg7_output_update();
    elapsed_call_back = func_ptr;
    scheduler_register_task(&task_decrement, egg_timer_decrement, ONE_SECOND);
}


/*
 * See header file
 */
void egg_timer_stop(void)
{
    // stop the timer
    scheduler_unregister_task(&task_decrement);
}


/*
 * See header file
 */
void egg_timer_resume(void)
{
    // resume the timer
    scheduler_register_task(&task_decrement, egg_timer_decrement, ONE_SECOND);
}


/*
 * See header file
 */
void egg_timer_get_output(seg7_output_t *output)
{
    output->value = time_get_bcd(&egg_timer);
    output->dots = (0x1 << 2u);     // Set dot on DS2
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Decrement timer variable.
 */
static void egg_timer_decrement(void)
{
    time_seconds_dec(&egg_timer);

    // output data
    seg7_output_update();

    if ((egg_timer.seconds == (uint8_t)0) &&
        (egg_timer.minutes == (uint8_t)0)) {
        elapsed_call_back();
    }
}
