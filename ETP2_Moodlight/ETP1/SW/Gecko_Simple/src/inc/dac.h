/** ***************************************************************************
 * @file
 * @brief See dac.c
 *****************************************************************************/

#ifndef DAC_H_
#define DAC_H_

/******************************************************************************
 * Functions
 *****************************************************************************/

void DAC0_init(void);
void DAC0_write(uint32_t);
void DAC0_start(uint32_t);
void DAC0_stop();

#endif
