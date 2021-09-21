/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module work.
 *
 *  $Id: work.c 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <stdint.h>
#include <stdlib.h>

/* User includes */
#include "work.h"
#include "output.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define WORK_REPETITION (45000u)


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void work_single_task(void)
{
    static volatile uint32_t t = 0x67d2e0cb;
    static volatile uint32_t u = 0xfb9202ab;

    t ^= u;
    u += t;
    t *= u;
    u -= t;
}


/*
 * See header file
 */
void work_hard(void)
{
    uint32_t r;

    for (r = 0; r < WORK_REPETITION; r++) {
        work_single_task();
    }
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

