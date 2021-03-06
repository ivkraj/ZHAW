/***************************************************************************//**
 * @file
 * @brief Voltage Comparator (VCMP) peripheral API
 * @version 5.8.1
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "em_vcmp.h"
#if defined(VCMP_COUNT) && (VCMP_COUNT > 0)

#include "em_assert.h"

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup VCMP
 * @brief Voltage Comparator (VCMP) Peripheral API
 * @details
 *  This module contains functions to control the VCMP peripheral of Silicon
 *  Labs 32-bit MCUs and SoCs. The VCMP monitors the input voltage supply and
 *  generates interrupts on events using as little as 100 nA.
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Configure and enable Voltage Comparator.
 *
 * @param[in] vcmpInit
 *   The VCMP Initialization structure.
 ******************************************************************************/
void VCMP_Init(const VCMP_Init_TypeDef *vcmpInit)
{
  /* Verify input. */
  EFM_ASSERT((vcmpInit->inactive == 0) || (vcmpInit->inactive == 1));
  EFM_ASSERT((vcmpInit->biasProg >= 0) && (vcmpInit->biasProg < 16));

  /* Configure the Half Bias setting. */
  if (vcmpInit->halfBias) {
    VCMP->CTRL |= VCMP_CTRL_HALFBIAS;
  } else {
    VCMP->CTRL &= ~(VCMP_CTRL_HALFBIAS);
  }

  /* Configure the bias prog. */
  VCMP->CTRL &= ~(_VCMP_CTRL_BIASPROG_MASK);
  VCMP->CTRL |= (vcmpInit->biasProg << _VCMP_CTRL_BIASPROG_SHIFT);

  /* Configure sense for the falling edge. */
  if (vcmpInit->irqFalling) {
    VCMP->CTRL |= VCMP_CTRL_IFALL;
  } else {
    VCMP->CTRL &= ~(VCMP_CTRL_IFALL);
  }

  /* Configure sense for the rising edge. */
  if (vcmpInit->irqRising) {
    VCMP->CTRL |= VCMP_CTRL_IRISE;
  } else {
    VCMP->CTRL &= ~(VCMP_CTRL_IRISE);
  }

  /* Configure the warm-up time. */
  VCMP->CTRL &= ~(_VCMP_CTRL_WARMTIME_MASK);
  VCMP->CTRL |= (vcmpInit->warmup << _VCMP_CTRL_WARMTIME_SHIFT);

  /* Configure hysteresis. */
  switch (vcmpInit->hyst) {
    case vcmpHyst20mV:
      VCMP->CTRL |= VCMP_CTRL_HYSTEN;
      break;
    case vcmpHystNone:
      VCMP->CTRL &= ~(VCMP_CTRL_HYSTEN);
      break;
    default:
      break;
  }

  /* Configure the inactive output value. */
  VCMP->CTRL |= (vcmpInit->inactive << _VCMP_CTRL_INACTVAL_SHIFT);

  /* Configure the trigger level. */
  VCMP_TriggerSet(vcmpInit->triggerLevel);

  /* Enable or disable VCMP. */
  if (vcmpInit->enable) {
    VCMP->CTRL |= VCMP_CTRL_EN;
  } else {
    VCMP->CTRL &= ~(VCMP_CTRL_EN);
  }

  /* If Low Power Reference is enabled, wait until VCMP is ready */
  /* before enabling it. See the reference manual for deatils.        */
  /* Configuring Low Power Ref without enable has no effect.      */
  if (vcmpInit->lowPowerRef && vcmpInit->enable) {
    /* Poll for VCMP ready. */
    while (!VCMP_Ready()) ;
    VCMP_LowPowerRefSet(vcmpInit->lowPowerRef);
  }

  /* Clear the edge interrupt. */
  VCMP_IntClear(VCMP_IF_EDGE);
}

/***************************************************************************//**
 * @brief
 *    Enable or disable Low Power Reference setting.
 *
 * @param[in] enable
 *    If true, enables low power reference. If false, disable low power reference.
 ******************************************************************************/
void VCMP_LowPowerRefSet(bool enable)
{
  if (enable) {
    VCMP->INPUTSEL |= VCMP_INPUTSEL_LPREF;
  } else {
    VCMP->INPUTSEL &= ~VCMP_INPUTSEL_LPREF;
  }
}

/***************************************************************************//**
 * @brief
 *    Configure the trigger level of the voltage comparator.
 *
 * @param[in] level
 *    A trigger value, in range 0-63.
 ******************************************************************************/
void VCMP_TriggerSet(int level)
{
  /* Trigger range is 6 bits, value from 0-63. */
  EFM_ASSERT((level > 0) && (level < 64));

  /* Set the trigger level. */
  VCMP->INPUTSEL = (VCMP->INPUTSEL & ~(_VCMP_INPUTSEL_TRIGLEVEL_MASK))
                   | (level << _VCMP_INPUTSEL_TRIGLEVEL_SHIFT);
}

/** @} (end addtogroup VCMP) */
/** @} (end addtogroup emlib) */
#endif /* defined(VCMP_COUNT) && (VCMP_COUNT > 0) */
