/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Interface of module observable.
 * 
 *  $Id: observable.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _OBSERVABLE_H
#define _OBSERVABLE_H


/* Standard includes */
#include <stdint.h>


/* -- Macros
 * ------------------------------------------------------------------------- */

#define MAX_NR_OF_OBSERVERS 5


/* -- Type definitions
 * ------------------------------------------------------------------------- */

/**
 *  \struct observable_t
 *  \brief  Observable type containing an array of pointers to callback 
 *  functions. The array is filled up from the beginning without gaps. 
 *  next_observer_nr indicates the next free index that is available.
 *  :TODO: init?
 */
typedef struct {
    void(*func_ptr[MAX_NR_OF_OBSERVERS])(uint32_t data);
    uint32_t next_observer_nr;
} observable_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initializes observable object.
 *  \param  observable : Observable object.
 */
void observable_init(observable_t *observable);

/**
 *  \brief  Register a callback function that will be called through notify().
 *  \param  observable : Observable object for which the callback shall be
 *                       registered.
 *  \param  func_ptr : Pointer to the callback function that will be 
 *                     registered.
 *  \return 0 for success / 1 in case of error (out of space).
 */
uint32_t observer_register_observer(observable_t *observable,
                                    void (*func_ptr)(uint32_t data));

/**
 *  \brief  Notifies all observers by calling all registered callbacks.
 *  \param  message : Value passed to the observer when calling the callback.
 */
void observable_notify(observable_t *observable, uint32_t message);

#endif
