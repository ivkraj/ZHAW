/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 labs
 * -- Description : This module provides base functionality for the
 * --               MC1 filesystem board
 * --
 * -- $Id: mc1_fs_board.h 2067 2020-10-05 11:23:44Z ruan $
 * --------------------------------------------------------------- */

#ifndef __gpio_H
#define __gpio_H

/* LED0 - LED3 on additional HW. Mapped on GPIOE */
#define SET_LED0_ON  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET)
#define SET_LED1_ON  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET)
#define SET_LED2_ON  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET)
#define SET_LED3_ON  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

#define SET_LED0_OFF HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET)
#define SET_LED1_OFF HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_LED2_OFF HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET)
#define SET_LED3_OFF HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)

/* Switch S0-S3 on additional HW. Mapped on GPIOD */
#define READ_S0_STATE HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_3)
#define READ_S1_STATE HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_4)
#define READ_S2_STATE HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_5)
#define READ_S3_STATE HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6)

/* Button T0-T3 on additional HW. Mapped on GPIOD */
#define READ_T0_STATE     HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_0)
#define READ_T1_STATE     HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_1)
#define READ_T2_STATE     HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_2)
#define READ_T3_STATE     HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3)

/**
 * \brief Initialize the inputs and outputs of the MC1 filesystem board
 *        LEDs, switches and buttons
 *
 */
void mc1_fs_board_init(void);

#endif
