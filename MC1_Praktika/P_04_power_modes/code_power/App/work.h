/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module work.
 *
 *  $Id: work.h 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _WORK_H
#define _WORK_H


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Do some calculations.
 */
void work_single_task(void);

/**
 *  \brief  Repetivly do some calculations for a defined cpu load.
 */
void work_hard(void);
#endif
