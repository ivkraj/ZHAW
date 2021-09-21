/** ***************************************************************************
 * @file
 * @brief See gpioInterrupt.c
 *****************************************************************************/

#ifndef GPIOINTERRUPT_H_
#define GPIOINTERRUPT_H_

/******************************************************************************
 * Defines
 *****************************************************************************/
// LM75 OS
#define PD0_PORT    gpioPortD
#define PD0_PIN     0

// OPT3001 INT
#define PC5_PORT	gpioPortC
#define PC5_PIN		5

// BT_CON INT
#define PE1_PORT    gpioPortE
#define PE1_PIN     1

/******************************************************************************
 * Functions
 *****************************************************************************/

void GPIOINT_init(void);
void GPIOINT_LumINT(void);
void GPIOINT_TempOS(void);
bool GPIOgetBTState(void);

#endif
