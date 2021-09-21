/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module observable.
 * 
 *  $Id: observable.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "observable.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define SUCCESS             (0u)
#define FAIL                (1u)


/* -- Public function definitions
 * ------------------------------------------------------------------------- */
 
/**
 *  See header file
 */
void observable_init(observable_t *observable)
{
    observable->next_observer_nr = 0;
}


/**
 *  See header file
 */
uint32_t observer_register_observer(observable_t *observable,
                                    void (*func_ptr)(uint32_t data))
{
    if (observable->next_observer_nr < MAX_NR_OF_OBSERVERS) {
        observable->func_ptr[observable->next_observer_nr] = func_ptr;
        observable->next_observer_nr++;
        return SUCCESS;
    } else {
        return FAIL;   // error out of function pointers
    }
}


/**
 *  See header file
 */
void observable_notify(observable_t *observable, uint32_t message)
{
    uint32_t i;

    for (i = 0; i < observable->next_observer_nr; i++) {
        observable->func_ptr[i](message);
    }
}
