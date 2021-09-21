/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Interface of module queue.
 * 
 *  $Id: queue.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _QUEUE_H
#define _QUEUE_H


/* Standard includes */
#include <stdint.h>


/* -- Macros
 * ------------------------------------------------------------------------- */

#define QUEUE_SIZE          (20u)
#define QUEUE_EMPTY_VALUE   (0u)


/* -- Type definitions
 * ------------------------------------------------------------------------- */

/**
 *  \struct queue_t
 *  \brief  Defines a queue.
 */
typedef struct {
    uint32_t head;
    uint32_t tail;
    uint32_t queue_data[QUEUE_SIZE];
} queue_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Enqueues data at the tail of the specified queue
 *  \param  queue   Points to the queue to which the element should be added
 *  \param  data
 *  \return 0 if data NOT added (queue is full), 
 *          ~0 if data added (queue not full)
 */
uint32_t queue_enqueue(queue_t *queue, uint32_t data);

/**
 *  \brief  Grabs and removes the element at the head of the specified queue
 *  \param  queue   Points to the queue from which the element should be polled
 *  \return Queue element, returns 0 if no element in queue.
 */
uint32_t queue_dequeue(queue_t *queue);

#endif
