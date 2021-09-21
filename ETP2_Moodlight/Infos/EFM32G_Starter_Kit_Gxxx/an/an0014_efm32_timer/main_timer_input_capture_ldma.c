/*****************************************************************************
 * @file main_timer_input_capture_dma.c
 * @brief TIMER Input Capture with DMA Demo Application
 * @author Silicon Labs
 * @version 1.10
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
/*
 * This program measures how long PB0 is pressed and transfers the value via LDMA
 * to bufferA and bufferB.  This program is intended for EFM32PG and EFM32JG
 * (STK3401).
 */
#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_prs.h"
#include "em_ldma.h"
#include "em_system.h"
#include "em_timer.h"
#include "em_chip.h"
#include "stdio.h"
#include "dmactrl.h"

#define DMA_CHANNEL_TIMER0       0
#define BUFFERSIZE               8

volatile uint16_t bufferA[BUFFERSIZE];
volatile uint16_t bufferB[BUFFERSIZE];

/* TOP reset value is 0xFFFF so it doesn't need 
   to be written for this example */
#define TOP 0xFFFF

/* 13761 Hz -> 14Mhz (clock frequency) / 1024 (prescaler) */
#define TIMER_FREQ 13671

#if defined ( STK3700 ) || ( STK3300 ) || ( STKG8XX ) || ( STK3600 ) || ( STK3200 ) || ( STK3400 )
  #error "This kit does not support LDMA"
#elif defined ( STK3401 )
  #define PB0_PORT gpioPortF
  #define PB0_PIN 6
  #define PB0_PRS_SIGSEL_PIN PRS_CH_CTRL_SIGSEL_GPIOPIN6
#else
  #error "undefined KIT"
#endif


/***************************************************************************//**
 * @brief  LDMA IRQ handler.
 ******************************************************************************/
void LDMA_IRQHandler( void )
{
  uint32_t pending, chnum, chmask;

  /* Get all pending and enabled interrupts */
  pending  = LDMA->IF;
  pending &= LDMA->IEN;

  /* Check for LDMA error */
  if ( pending & LDMA_IF_ERROR )
  {
    /* Loop here to enable the debugger to see what has happened */
    while (1)
      ;
  }

  /* Iterate over all LDMA channels. */
  for ( chnum = 0,                chmask = 1;
        chnum < DMA_CHAN_COUNT;
        chnum++,                  chmask <<= 1 )
  {
    if ( pending & chmask )
    {
      /* Clear interrupt flag. */
      LDMA->IFC = chmask;

      /* Do more stuff here, execute callbacks etc. */
    }
  }
}

/**
 * @brief
 *   DMA descriptor initializer for byte transfers from a peripheral to memory.
 * @param[in] src       Peripheral data source register address.
 * @param[in] dest      Destination data address.
 * @param[in] count     Number of bytes to transfer.
 * @param[in] linkjmp   Address of descriptor to link to expressed as a
 *                      signed number of descriptors from "here".
 *                      1=one descriptor forward in memory,
 *                      0=one this descriptor,
 *                      -1=one descriptor back in memory.
 */
#define LDMA_DESCRIPTOR_LINKREL_P2M_HALF( src, dest, count, linkjmp ) \
{                                                                     \
  .xfer =                                                             \
  {                                                                   \
    .structType   = ldmaCtrlStructTypeXfer,                           \
    .structReq    = 0,                                                \
    .xferCnt      = ( count ) - 1,                                    \
    .byteSwap     = 0,                                                \
    .blockSize    = ldmaCtrlBlockSizeUnit1,                           \
    .doneIfs      = 1,                                                \
    .reqMode      = ldmaCtrlReqModeBlock,                             \
    .decLoopCnt   = 0,                                                \
    .ignoreSrec   = 0,                                                \
    .srcInc       = ldmaCtrlSrcIncNone,                               \
    .size         = ldmaCtrlSizeHalf,                                 \
    .dstInc       = ldmaCtrlDstIncOne,                                \
    .srcAddrMode  = ldmaCtrlSrcAddrModeAbs,                           \
    .dstAddrMode  = ldmaCtrlDstAddrModeAbs,                           \
    .srcAddr      = (uint32_t)(src),                                  \
    .dstAddr      = (uint32_t)(dest),                                 \
    .linkMode     = ldmaLinkModeRel,                                  \
    .link         = 1,                                                \
    .linkAddr     = ( linkjmp ) * 4                                   \
  }                                                                   \
}

const LDMA_TransferCfg_t periTransferRx = LDMA_TRANSFER_CFG_PERIPHERAL( ldmaPeripheralSignal_TIMER0_CC0 );
const LDMA_Descriptor_t xfer[] = {
  LDMA_DESCRIPTOR_LINKREL_P2M_HALF( &(TIMER0->CC[0].CCV),
                                    bufferA,
                                    BUFFERSIZE,
                                    1),
  LDMA_DESCRIPTOR_LINKREL_P2M_HALF( &(TIMER0->CC[0].CCV),
                                    bufferB,
                                    BUFFERSIZE,
                                    -1)
};

/**************************************************************************//**
 * @brief Configure DMA for Ping-Pong transfers
 *****************************************************************************/
void setupDma(void)
{
  LDMA_Init_t init = LDMA_INIT_DEFAULT;
  
  LDMA_Init( &init );
  LDMA_StartTransfer( 0, (void*)&periTransferRx, (void*)&xfer );

}


/**************************************************************************//**
 * @brief  Main function
 * Main is called from __iar_program_start, see assembly startup file
 *****************************************************************************/
int main(void)
{  
  /* Initialize chip */
  CHIP_Init();
    
  /* Enable clock for GPIO module */
  CMU_ClockEnable(cmuClock_GPIO, true);
  
  /* Enable clock for TIMER0 module */
  CMU_ClockEnable(cmuClock_TIMER0, true);
   
  /* Enable clock for PRS module */
  CMU_ClockEnable(cmuClock_PRS, true);
  
  /* Setup DMA */
  setupDma();
  
  /* Configure PB0_PIN as an input for PB0 button with filter and pull-up (dout = 1)*/
  GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeInputPullFilter, 1); 
  
  /* Select PB0_PIN as external interrupt source*/
  GPIO_IntConfig(PB0_PORT, PB0_PIN, false, false, false);
  
  /* Select GPIO as source and PB0_PRS_SIGSEL_PIN as signal for PRS channel 0 */
  PRS_SourceSignalSet(0, PRS_CH_CTRL_SOURCESEL_GPIOL, PB0_PRS_SIGSEL_PIN, prsEdgeOff);
    
  /* Select CC channel parameters */
  TIMER_InitCC_TypeDef timerCCInit = 
  {
    .eventCtrl  = timerEventEveryEdge,
    .edge       = timerEdgeRising,
    .prsSel     = timerPRSSELCh0,
    .cufoa      = timerOutputActionNone,
    .cofoa      = timerOutputActionNone,
    .cmoa       = timerOutputActionNone,
    .mode       = timerCCModeCapture,
    .filter     = true,
    .prsInput   = true,
    .coist      = false,
    .outInvert  = false,
  };
  
  /* Configure CC channel 0 */
  TIMER_InitCC(TIMER0, 0, &timerCCInit);

  /* Select timer parameters */  
  TIMER_Init_TypeDef timerInit =
  {
    .enable     = false,
    .debugRun   = true,
    .prescale   = timerPrescale1024,
    .clkSel     = timerClkSelHFPerClk,
    .fallAction = timerInputActionReloadStart,
    .riseAction = timerInputActionStop,
    .mode       = timerModeUp,
    .dmaClrAct  = false,
    .quadModeX4 = false,
    .oneShot    = false,
    .sync       = false,
  };
  
  /* Configure timer */
  TIMER_Init(TIMER0, &timerInit);
   
  while(1)
  {
    /* Go to EM1 */
    EMU_EnterEM1();
  }
  
}
