/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab file system
 * -- Description : This code controls the accelerometer LSM6DSL
 * --               and writes its values on the SD card
 * --
 * -- $Id: main.c 2067 2020-10-05 11:23:44Z ruan $
 * --------------------------------------------------------------- */

#include "sdio.h"
#include "spi.h"
#include "display.h"
#include "mc1_fs_board.h"
#include "accelerometer.h"
#include "file_io.h"
#include "error.h"
#include <string.h>
#include <stdio.h>

#define NR_OF_VALUES              3
#define FILE_NAME_LENGTH         25
#define SAMPLES_FOR_AVERAGE     100

/* global variables ----------------------------------------------------------*/
volatile uint8_t data_ready;            // indicates that an interrupt occured
                                        // shared with EXTI9_5_IRQHandler(void)
                                        // in module stm32f4xx_it

/* function prototypes -------------------------------------------------------*/
static void init_system(void);
static void SystemClock_Config(void);
static void calculate_acc_average(int16_t *buffer, int16_t *result,
                                  uint16_t buffer_length);
static mode_t read_mode(void);

/* MAIN FUNCTION ------------------------------------------------------------ */
int main(void)
{
    int16_t acceleration_avg[NR_OF_VALUES];        // average values
    int16_t acc_buffer[SAMPLES_FOR_AVERAGE * NR_OF_VALUES];
    uint16_t nr_of_samples = 0;     // indicates how many samples (x,y,z value)
                                    // are currently in the buffer

    FILE_IO_STATUS file_io_status;
    uint8_t who_am_i;
    mode_t mode;
    
    data_ready = 0;
    
    /* initialize the MCU infrastructure */
    init_system();
    
    /* initialize the sensor based on the selected mode */
    mode = read_mode();
    accelerometer_init(mode);
    
    /* register file system using function file_io_register_filesystem() */
    /// STUDENTS: To be programmed
    
    halt_on_file_io_error(file_io_register_filesystem());
    SET_LED0_ON;

    /// END: To be programmed    

    /* create files with function file_io_create() and handle errors */
    /// STUDENTS: To be programmed
    
    halt_on_file_io_error(file_io_create(FILE_IO_AVERAGE));
    halt_on_file_io_error(file_io_create(FILE_IO_RAW));
    SET_LED1_ON;

    /// END: To be programmed
      
    /* read and print ID of accelerometer */    
    who_am_i = accelerometer_who_am_i();
    display_write_who_am_I(who_am_i);

    while (!READ_T0_STATE) {
        if (data_ready > 0) {
            
            data_ready = 0; // reset flag early to avoid missing of interrupts
            
            switch (mode) {
                case SPI_SINGLE: case SPI_DMA:
                    
                    /* Call accelerometer_read_acceleration() and store data
                     * in the array defined above called acc_buffer
                     * Use nr_of_samples as an index
                     */
                    /// STUDENTS: To be programmed
                
                    accelerometer_read_acceleration(&acc_buffer[nr_of_samples*3], mode);
                    
                    /// END: To be programmed
                
                    nr_of_samples += 1;             
                
                    /* wait until specified number of samples have been received, 
                     * so display doesn't refresh for every sample */
                    if (nr_of_samples >= SAMPLES_FOR_AVERAGE) {
                        
                        /* calculate average values from acc_buffer*/
                        calculate_acc_average(acc_buffer, 
                                              acceleration_avg, nr_of_samples);

                        /* display the acceleration */
                        display_write_text_and_data(acceleration_avg[0], 
                                                    acceleration_avg[1],
                                                    acceleration_avg[2]);
                        nr_of_samples = 0;
                    }

                 
                    break;
                
                case SPI_SINGLE_LSM_FIFO: case SPI_DMA_LSM_FIFO:
                    
                    /* Read the FIFO, calculate the average and display the
                     * the average on the lcd 
                     */
                    /// STUDENTS: To be programmed
                    nr_of_samples = accelerometer_read_acceleration(acc_buffer, mode);
                
                    /* calculate average values from acc_buffer*/
                    calculate_acc_average(acc_buffer, 
                                          acceleration_avg, nr_of_samples);

                    /* display the acceleration */
                    display_write_text_and_data(acceleration_avg[0], 
                                                acceleration_avg[1],
                                                acceleration_avg[2]);
                
                    /*for(uint16_t i = 0; i < 3; i++){
                        acceleration_avg[i]=i;
                    }
                    for(uint32_t i = 0; i < nr_of_samples*3; i++){
                        acc_buffer[i]=i;
                    }*/
                    
                    file_io_write_raw_to_sd_card(acc_buffer, nr_of_samples);
                    file_io_write_avg_to_sd_card(acceleration_avg);


                    /// END: To be programmed
                
                default:
                    break;
            }
        }
    }

    /* Unregister filesystem object from the fatfs module */
    
    /* Use function file_io_unregister_filesystem() and handle errors */
    
    /// STUDENTS: To be programmed
    
    halt_on_file_io_error(file_io_unregister_filesystem());
    SET_LED0_OFF;
    SET_LED1_OFF;

    /// END: To be programmed
    
    while(1); // loop forever
}

/**
 *  \brief  calculate the average of a buffer filled with 3 axis values
 *  \param  samples: a buffer with samples consisting of 3-axis values
 *  \param  result: array with 3 positions on
 *                  which the result will be stored
 *  \param  nr_of_samples: length of the input buffer
 */
static void calculate_acc_average(int16_t *samples, int16_t *result,
                                  uint16_t nr_of_samples)
{
    
    /* Copy the first sample of the buffer to the result as default behavior
     * before implementing this function.
     */
    result[0] = samples[0];
    result[1] = samples[1];
    result[2] = samples[2];
    
    int32_t sum_x = 0;
    int32_t sum_y = 0;
    int32_t sum_z = 0;
    
    /// STUDENTS: To be programmed
    
    for(uint16_t i = 0; i < nr_of_samples; i++){
        sum_x += samples[3*i];
        sum_y += samples[3*i+1];
        sum_z += samples[3*i+2];
    }
    result[0] = sum_x / nr_of_samples;
    result[1] = sum_y / nr_of_samples;
    result[2] = sum_z / nr_of_samples;
    
    /// END: To be programmed
}

/**
 *  \brief  performs all the configurations and initializations
 */
static void init_system(void)
{
    /* STM32F4xx HAL library initialization:
        - Configure the Flash prefetch, instruction and Data caches
        - Configure the Systick to generate an interrupt each 1 msec
        - Set NVIC Group Priority to 4
        - Global MSP (MCU Support Package) initialization
     */
    SPI_STATUS spi_status;
    
    HAL_Init();

    /* Configure the system clock to 96 MHz */
    SystemClock_Config();
    
    display_init();
    mc1_fs_board_init();
    spi_status = MX_SPI_Init();
    halt_on_spi_error(spi_status);
    MX_SDIO_SD_Init();
    MX_FATFS_Init();
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 96000000
 *            HCLK(Hz)                       = 96000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 4
 *            PLL_N                          = 192
 *            PLL_P                          = 4
 *            PLL_Q                          = 8
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 *         The LTDC Clock is configured as follow :
 *            PLLSAIN                        = 192
 *            PLLSAIR                        = 4
 *            PLLSAIDivR                     = 16
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the
       device is clocked below the maximum system frequency, to update the
       voltage scaling value regarding system frequency refer to product
       datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /*##-1- System Clock Configuration #######################################*/
    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4; // PLLCLK = 96 MHz
    RCC_OscInitStruct.PLL.PLLQ = 8;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                   RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // SYSCLK = 96 MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;       // PCLK1 = 24 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;       // PCLK2 = 48 MHz
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    /*##-2- LTDC Clock Configuration #########################################*/
    /* LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 2 MHz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 MHz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384/4 = 96 MHz */
    /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDIVR_16 = 96/16 = 6 MHz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_16;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    
      /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
}

/**
 *  \brief  Reads the dip switches to set the mode
 */
static mode_t read_mode(void)
{
    mode_t mode;
    uint32_t uses_dma;
    uint32_t uses_fifo;
    uses_dma = READ_S0_STATE;
    uses_fifo = READ_S1_STATE;
    
    // switches are inverted
    if (uses_fifo && uses_dma) {
        mode = SPI_SINGLE;
        display_write_text(0, "SPI_SINGLE    ");
    } else if (uses_fifo && !uses_dma) {
        mode = SPI_DMA;
        display_write_text(0, "SPI_DMA       ");
    } else if (!uses_fifo && uses_dma) {
        mode = SPI_SINGLE_LSM_FIFO;
        display_write_text(0, "SPI_LSM_FIFO  ");
    } else if (!uses_fifo && !uses_dma) {
        mode = SPI_DMA_LSM_FIFO;
        display_write_text(0, "SPI_DMA_FIFO  ");
    }
    return mode;
}
