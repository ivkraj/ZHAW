/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Main program
 *
 *  $Id: main.c 1072 2016-11-18 12:50:47Z ruan $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <stdint.h>

/* User includes */
#include "scheduler.h"
#include "buttons.h"
#include "egg_timer_ctrl.h"
#include "reload_ctrl.h"
#include "mode_control.h"
#include "stop_watch_ctrl.h"
#include "button_count.h"
#include "seg7_display.h"
#include "hal_pwr.h"


/* -- Local variables
 * ------------------------------------------------------------------------- */

static scheduler_task_t task_buttons;
static scheduler_task_t task_mode_control;
static scheduler_task_t task_egg_timer_ctrl;
static scheduler_task_t task_reload_ctrl;
static scheduler_task_t task_stop_watch_ctrl;
static scheduler_task_t task_button_count_ctrl;


/* -- M A I N
 * ------------------------------------------------------------------------- */

int main(void)
{
    buttons_init();
    mode_control_init();
    
    /* Initialize scheduler */
    scheduler_init();

    /* Register tasks */
    scheduler_register_task(&task_buttons, buttons_check_pressed, TEN_MILLISECONDS);
    scheduler_register_task(&task_mode_control, mode_control_handle_event, TEN_MILLISECONDS);
    scheduler_register_task(&task_egg_timer_ctrl, et_ctrl_handle_event, TEN_MILLISECONDS);
    /// STUDENTS: To be programmed
    scheduler_register_task(&task_reload_ctrl, rl_ctrl_handle_event, TEN_MILLISECONDS);
    scheduler_register_task(&task_stop_watch_ctrl, sw_ctrl_handle_event, TEN_MILLISECONDS);
    scheduler_register_task(&task_button_count_ctrl, button_count_handle_event, TEN_MILLISECONDS);

    /// END: To be programmed

    while (1) {
        // endless loop
    }
}
