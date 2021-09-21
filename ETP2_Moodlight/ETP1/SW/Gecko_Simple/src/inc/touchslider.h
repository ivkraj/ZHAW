/** ***************************************************************************
 * @file
 * @brief See touchslider.c
 *****************************************************************************/

#ifndef TOUCHSLIDER_H_
#define TOUCHSLIDER_H_

/* Include the standard functionality of the capacitive sense system */
#include "capsense.h"

/******************************************************************************
 * Variables
 *****************************************************************************/

extern volatile bool touchsliderFlag;

/******************************************************************************
 * Functions
 *****************************************************************************/

int32_t CAPSENSE_getSliderValue(int32_t sliderMin, int32_t sliderMax);

#endif
