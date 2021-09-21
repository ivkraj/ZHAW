/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module queue.
 * 
 *  $Id: queue.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "queue.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define FALSE               (0u)
#define TRUE                (~FALSE)


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
uint32_t queue_enqueue(queue_t *queue, uint32_t data)
{
    uint32_t ret_val = TRUE;

    if ((queue->head == queue->tail + 1) ||
        ((queue->head == 0) && ((queue->tail + 1) == QUEUE_SIZE))) {
        // queue full
        ret_val = FALSE;
    } else {
        queue->queue_data[queue->tail] = data;
        queue->tail++;
        if (queue->tail == QUEUE_SIZE) {
            queue->tail = 0;
        }
    }

    return ret_val;
}


/*
 * See header file
 */
uint32_t queue_dequeue(queue_t *queue)
{
    uint32_t ret_val;

    if (queue->head == queue->tail) {
        // queue empty
        ret_val = QUEUE_EMPTY_VALUE;
    } else {
        ret_val = queue->queue_data[queue->head];
        queue->head++;
        if (queue->head == QUEUE_SIZE) {
            queue->head = 0;
        }
    }

    return ret_val;
}
