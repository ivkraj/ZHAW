/* -----------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- main.c
 * --
 * -- main for MC1 lab on debouncing
 * --
 * -- $Id: main.c 1537 2018-09-14 07:02:20Z kjaz $
 * ------------------------------------------------------------------
 */
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "reg_ctboard.h"

#define BITMASK_KEY_0       0x01
#define BITMASK_KEY_1       0x02
#define BITMASK_KEY_2       0x04
#define BITMASK_KEY_3       0x08
#define BITMASK_KEY_ALL     0x0F

#define NR_OF_7SEG_DISPLAYS 4u
#define NR_SAMPLES          50  // set to 50 for stable generic debounce

#define GPIO_P6             (uint16_t)0x0040
#define GPIO_P3_to_P0       (uint16_t)0x000F

/*  typedef used for task 4.4 */
typedef struct {
    uint8_t samples[NR_SAMPLES];   // space to store the samples
    uint8_t write_index;            // index where next sample will be written
                                    // to array
} sample_buffer_t;

/* forward declarations of internal functions */
static uint8_t detect_switch_change_debounce(void);
static uint8_t detect_switch_change(void);
static uint8_t generic_debounce(sample_buffer_t *sample_buffer,
                                uint8_t new_sample);
static void output_7seg_digits(uint8_t values[]);

int main(void)
{
    uint8_t switch_change = 0;
    uint8_t count[NR_OF_7SEG_DISPLAYS];
    uint32_t i;
    sample_buffer_t generic_buffer;
    uint8_t state_switches = 0;

    hal_gpio_input_t gpio_init_input;
    hal_gpio_output_t gpio_init_output;
    
    /* additional variable definitions go here */
    /// STUDENTS: To be programmed




    /// END: To be programmed

    /* initialize counters */
    for (i = 0; i < NR_OF_7SEG_DISPLAYS; i++) {
        count[i] = 0u;
    }

    /* enable clock on GPIOA and GPIOB */
    GPIOA_ENABLE();
    GPIOB_ENABLE();

    GPIOA_RESET();
    GPIOB_RESET();

    /* configure GPIO pins --> task 4.2 */

    /* configure PB3 down to PB0 as inputs with pullups */
    gpio_init_input.pins = GPIO_P3_to_P0;
    gpio_init_input.pupd = HAL_GPIO_PUPD_UP;
    hal_gpio_init_input(GPIOB, gpio_init_input);

    /* configure PB6 as output --> monitor */
    gpio_init_output.pins = GPIO_P6;
    gpio_init_output.pupd = HAL_GPIO_PUPD_NOPULL;
    gpio_init_output.out_speed = HAL_GPIO_OUT_SPEED_2MHZ;
    gpio_init_output.out_type = HAL_GPIO_OUT_TYPE_PP;
    hal_gpio_init_output(GPIOB, gpio_init_output);

    hal_gpio_bit_set(GPIOB, 0x040);

    while (1) {
        switch (CT_DIPSW->BYTE.S7_0 & 0x03) {
            case    0x00:
                /* Task 4.2: Edge detection NO debounce */
                switch_change = detect_switch_change();
                break;
            case    0x01:
                /* Task 4.3: Edge detection WITH debounce */
                switch_change = detect_switch_change_debounce();
                break;
            default:
                /* Task 4.4: Edge detection WITH multiple extension boards */
                /// STUDENTS: To be programmed
                state_switches = (uint8_t) hal_gpio_input_read(GPIOB);
                switch_change = generic_debounce(&generic_buffer, state_switches);

                /// END: To be programmed
                break;
        }

        /* increment the appropriate counter if a key has been switched */
        if (switch_change < NR_OF_7SEG_DISPLAYS) {
            count[switch_change]++;
        }

        output_7seg_digits(count);
    }
}

/**
 *  \brief  Evaluates dip switch changes on PB[3:0] since last function call.
 *
 *  Evaluates whether one of the dip switches has changed since the last call
 *  to this function. The function takes a sample of ports PB[3:0] and compares
 *  it to the sample values taken during the previous function call. In cases
 *  where more than one dip_switch has changed, only the changed dip_switch
 *  with the lowest index is returned. The others are ignored.
 *
 *  \param  none
 *  \return 00  --> change on PB0
 *          01  --> change on PB1
 *          02  --> change on PB2
 *          03  --> change on PB3
 *          FF  --> no change
 *
 */
uint8_t detect_switch_change(void)
{
    static uint8_t previous_switch_values = 0x00;
    uint8_t switch_values;
    uint8_t changing_bits = 0x00;
    uint8_t ret_val = 0x00;

    /* make function entry visible on oscilloscope */
    //hal_gpio_bit_toggle(GPIOB, 0x040);
    
    // Schalter einlesen, Vergleichen mit vorherigen Schalterstellungen
    switch_values = (uint8_t) hal_gpio_input_read(GPIOB);
    changing_bits = previous_switch_values ^ switch_values;
    previous_switch_values = switch_values;

    /* This code could be moved to an own function to allow reuse.
       However this would make the pre-defined frame for the students more
       difficult to understand. */
    if (changing_bits & BITMASK_KEY_0) {
        ret_val = 0;
    } else if (changing_bits & BITMASK_KEY_1) {
        ret_val = 1;
    } else if (changing_bits & BITMASK_KEY_2) {
        ret_val = 2;
    } else if (changing_bits & BITMASK_KEY_3) {
        ret_val = 3;
    } else {
        ret_val = 0xFF;
    }
    return(ret_val);
}

/**
 *  \brief  Evaluates dip switch changes on PB[3:0] with debouncing.
 *
 *  The function keeps a window of the last NR_SAMPLES on port PB[3:0]. On each
 *  call a new sample is taken and the oldest sample is discarded. The window
 *  is then searched for rising and falling edges. The lowest index with an
 *  edge is returned. Other edges are ignored.
 *
 *  \param  none
 *  \return 00  --> change on PB0
 *          01  --> change on PB1
 *          02  --> change on PB2
 *          03  --> change on PB3
 *          FF  --> no change
 *
 */
static uint8_t detect_switch_change_debounce(void)
{
    /// STUDENTS: To be programmed
    static uint8_t switch_samples[NR_SAMPLES];
    uint8_t count_values[4] = {0,0,0,0};
    uint8_t static_bits = 0x00;
    uint8_t changing_bits = 0x00;
    uint8_t ret_val = 0x00;
    
    uint8_t i;
    uint8_t switch_values;
    

    /* make function entry visible on oscilloscope */
    //hal_gpio_bit_toggle(GPIOB, 0x040);

    switch_values = ((uint8_t) hal_gpio_input_read(GPIOB)) & BITMASK_KEY_ALL;   // Schalter einlesen
    static_bits = switch_values;
    for(i = (NR_SAMPLES-1); i > 0; i--) {                   // Werte im Array nach unten schieben
        switch_samples[i] = switch_samples[i-1];
    }
    switch_samples[0] = switch_values & BITMASK_KEY_ALL;    // Neuster Wert in Array abspeichern
    
    for(i = 1; i < NR_SAMPLES; i++) {                       // Aufsummieren der vergangenen Samples
        count_values[0] += switch_samples[i] & BITMASK_KEY_0;
        count_values[1] += (switch_samples[i] & BITMASK_KEY_1) >> 1;
        count_values[2] += (switch_samples[i] & BITMASK_KEY_2) >> 2;
        count_values[3] += (switch_samples[i] & BITMASK_KEY_3) >> 3;
    }
    
    for(i = 0; i < 4; i++) {                                // Ueberpruefen, ob saubere Flanke
        if (count_values[i] == 0) {
            static_bits &= ~(0x01 << i);
        }
        if (count_values[i] == (NR_SAMPLES-1)) {
            static_bits |= (0x01 << i);
        }
    }
    
    changing_bits = switch_values ^ static_bits;            // Flankenerkennung         
    
    /* This code could be moved to an own function to allow reuse.
       However this would make the pre-defined frame for the students more
       difficult to understand. */
    if (changing_bits & BITMASK_KEY_0) {
        ret_val = 0;
    } else if (changing_bits & BITMASK_KEY_1) {
        ret_val = 1;
    } else if (changing_bits & BITMASK_KEY_2) {
        ret_val = 2;
    } else if (changing_bits & BITMASK_KEY_3) {
        ret_val = 3;
    } else {
        ret_val = 0xFF;
    }
    return(ret_val);
        

    /// END: To be programmed
}

/**
 *  \brief  Maintain sample history and look for edges with debouncing applied.
 *
 *  On each call the oldest sample is discarded and new_sample is added to the
 *  history. The history window is then searched for rising and falling edges.
 *  The lowest index with an edge is returned. Other edges are ignored.
 *
 *  \param  sample_buffer   pointer to a struct allocated by caller
 *                          The struct is organized as a ring buffer. It 
 *                          contains the sample history and an index where a
 *                          new sample shall be inserted.
 *                           
 *  \param  new_sample      The current 4-bit sample as read from the GPIO pins
 *
 *  \return 00  --> edge on bit0
 *          01  --> edge on bit1
 *          02  --> edge on bit2
 *          03  --> edge on bit3
 *          FF  --> no edge
 *
 */
static uint8_t generic_debounce(sample_buffer_t *sample_buffer,
                                uint8_t new_sample)
{
    /// STUDENTS: To be programmed
    
    uint8_t count_values[4]={0,0,0,0};
    uint8_t static_bits;
    uint8_t changing_bits;
    uint8_t ret_val;
    
    uint8_t i;
    
 /* make function entry visible on oscilloscope */
    //hal_gpio_bit_toggle(GPIOB, 0x040);

    sample_buffer->samples[sample_buffer->write_index] = new_sample & BITMASK_KEY_ALL;    // Neuer Schalterwert in Array laden
    static_bits = sample_buffer->samples[sample_buffer->write_index];
    
    for(i = ((sample_buffer->write_index) + 1); i < NR_SAMPLES; i++) {    // Array bis ans Ende aufsummieren
        count_values[0] += sample_buffer->samples[i] & BITMASK_KEY_0;
        count_values[1] += (sample_buffer->samples[i] & BITMASK_KEY_1) >> 1;
        count_values[2] += (sample_buffer->samples[i] & BITMASK_KEY_2) >> 2;
        count_values[3] += (sample_buffer->samples[i] & BITMASK_KEY_3) >> 3;
    }
    
    for(i = 0; i < (sample_buffer->write_index); i++) {                 // Array von Anfang bis Index aufsummieren
        count_values[0] += sample_buffer->samples[i] & BITMASK_KEY_0;
        count_values[1] += (sample_buffer->samples[i] & BITMASK_KEY_1) >> 1;
        count_values[2] += (sample_buffer->samples[i] & BITMASK_KEY_2) >> 2;
        count_values[3] += (sample_buffer->samples[i] & BITMASK_KEY_3) >> 3;
    }
        
    for(i = 0; i < 4; i++) {                                            // Ueberpruefen ob saubere Flanke
        if (count_values[i] == 0) {
            static_bits &= ~(0x01 << i);
        }
        if (count_values[i] == (NR_SAMPLES-1)) {
            static_bits |= (0x01 << i);
        }
    }
    
    changing_bits = sample_buffer->samples[sample_buffer->write_index] ^ static_bits;                        // Flankenerkennung
    
    if (sample_buffer->write_index == (NR_SAMPLES-1)) {                 // Index aktualisieren
        sample_buffer->write_index = 0;
    } else {
        sample_buffer->write_index++;
    }
    
    /* This code could be moved to an own function to allow reuse.
       However this would make the pre-defined frame for the students more
       difficult to understand. */
    if (changing_bits & BITMASK_KEY_0) {
        ret_val = 0;
    } else if (changing_bits & BITMASK_KEY_1) {
        ret_val = 1;
    } else if (changing_bits & BITMASK_KEY_2) {
        ret_val = 2;
    } else if (changing_bits & BITMASK_KEY_3) {
        ret_val = 3;
    } else {
        ret_val = 0xFF;
    }
    return(ret_val);



    /// END: To be programmed
}

/**
 *  \brief  Write lower nibbles of the 4 values to the 7-seg displays
 *
 *  \param  values[]    An array of four 8-bit values to be displayed on 7-seg
 */
static void output_7seg_digits(uint8_t values[])
{
    /* array allows simple conversion from int to seven-segment code */
    const uint8_t seven_seg_patterns[16] = { 0xc0, 0xf9, 0xa4, 0xb0,
                                             0x99, 0x92, 0x82, 0xf8,
                                             0x80, 0x90, 0x88, 0x83,
                                             0xc6, 0xa1, 0x86, 0x8e };

    /* Write to 7-segment display */
    CT_SEG7->RAW.BYTE.DS0 = seven_seg_patterns[values[0] & 0x0F];
    CT_SEG7->RAW.BYTE.DS1 = seven_seg_patterns[values[1] & 0x0F];
    CT_SEG7->RAW.BYTE.DS2 = seven_seg_patterns[values[2] & 0x0F];
    CT_SEG7->RAW.BYTE.DS3 = seven_seg_patterns[values[3] & 0x0F];
}
