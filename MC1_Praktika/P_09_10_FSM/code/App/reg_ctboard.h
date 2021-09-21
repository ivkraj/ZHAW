/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Registries for CT Board.
 * 
 *  $Id: reg_ctboard.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
#ifndef _REG_CTBOARD_H
#define _REG_CTBOARD_H

#include <stdint.h>


/*  LED register
 * ------------------------------------------------------------------------- */

/**
 *  \union  reg_ct_led_t
 *  \brief  Representation of LED interface.
 */
typedef union {
    struct {
        volatile uint8_t LED7_0;    /**< Byte access to LED7..0. */
        volatile uint8_t LED15_7;   /**< Byte access to LED15..0. */
        volatile uint8_t LED23_16;  /**< Byte access to LED23_16. */
        volatile uint8_t LED31_24;  /**< Byte access to LED31_24. */
    } BYTE;
    struct {
        volatile uint16_t LED15_0;  /**< Halfword access to LED15..0. */
        volatile uint16_t LED31_16; /**< Halfword access to LED31..16. */
    } HWORD;
    volatile uint32_t WORD;         /**< Word access to all LEDs. */
} reg_ct_led_t;


#define CT_LED      ((reg_ct_led_t *) 0x60000100)


/*  7 segment display register
 * ------------------------------------------------------------------------- */

/**
 *  \union  reg_ct_seg7_raw_t
 *  \brief  Representation of 7 segment display RAW interface.
 */
typedef union {
    struct {
        volatile uint8_t DS0;       /**< Byte access to DS0 segments. */
        volatile uint8_t DS1;       /**< Byte access to DS1 segments. */
        volatile uint8_t DS2;       /**< Byte access to DS2 segments. */
        volatile uint8_t DS3;       /**< Byte access to DS3 segments. */
    } BYTE;
    struct {
        volatile uint16_t DS1_0;    /**< Halfword access to DS1/0 segments. */
        volatile uint16_t DS3_2;    /**< Halfword access to DS3/2 segments. */
    } HWORD;
    volatile uint32_t WORD;         /**< Word access to all segments. */
} reg_ct_seg7_raw_t;


/**
 *  \union  reg_ct_seg7_bin_t
 *  \brief  Representation of 7 segment display BINary interface.
 */
typedef union {
    struct {
        volatile uint8_t DS1_0; /**< Byte access to binary display DS1..0. */
        volatile uint8_t DS3_2; /**< Byte access to binary display DS3..2. */
    } BYTE;
    volatile uint16_t HWORD;    /**< Halfword access to binary display. */
} reg_ct_seg7_bin_t;
	

/**
 *  \struct reg_ct_seg7_t
 *  \brief  Representation of 7 segment display register.
 *
 *  Described in ct board manual p.?ff.
 */
typedef struct {
    reg_ct_seg7_raw_t RAW;      /**< RAW interface. */
    uint8_t RESERVED[12];
    reg_ct_seg7_bin_t BIN;      /**< BINary interface. */
} reg_ct_seg7_t;


#define CT_7SEG     ((reg_ct_seg7_t *) 0x60000110)


/*  DIP switch register
 * ------------------------------------------------------------------------- */

/**
 *  \union  reg_ct_dipsw_t
 *  \brief  Representation of DIP switch interface.
 */
typedef union {
    struct {
        volatile uint8_t S7_0;      /**< Byte access to switches S7..0. */
        volatile uint8_t S15_7;     /**< Byte access to switches S15..8. */
        volatile uint8_t S23_16;    /**< Byte access to switches S23..16. */
        volatile uint8_t S31_24;    /**< Byte access to switches S31..24. */
    } BYTE;
    struct {
        volatile uint16_t S15_0;    /**< Byte access to switches S15..0. */
        volatile uint16_t S31_16;   /**< Byte access to switches S31..16. */
    } HWORD;
    volatile uint32_t WORD;         /**< Byte access to all switches. */
} reg_ct_dipsw_t;


#define CT_DIPSW    ((reg_ct_dipsw_t *) 0x60000200)


/*  BUTTON register
 * ------------------------------------------------------------------------- */

#define CT_BUTTON   ((uint8_t *) 0x60000210)


/*  HEX switch register
 * ------------------------------------------------------------------------- */

#define CT_HEXSW    ((uint8_t *) 0x60000211)


/*  LCD register
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_ct_lcd_i2c_t
 *  \brief  Representation of LCD i2c interface.
 */
typedef struct {
    volatile uint8_t CR;        /** Control register. */
    volatile uint8_t AR;        /** Address register. */
    volatile uint8_t DOR;       /** Data output register. */
    volatile uint8_t DIR;       /** Data input register. */
} reg_ct_lcd_i2c_t;
	

/**
 *  \struct reg_ct_lcd_bg_t
 *  \brief  Representation of LCD background interface.
 */
typedef struct {
    volatile uint16_t RED;      /** Red PWM duty cycle register. */
    volatile uint16_t GREEN;    /** Green PWM duty cycle register. */
    volatile uint16_t BLUE;     /** Blue PWM duty cycle register. */
} reg_ct_lcd_bg_t;


/**
 *  \union  reg_ct_lcd_bin_t
 *  \brief  Representation of LCD BINary interface.
 */
typedef union {
    struct {
        volatile uint8_t B7_0;      /**< Byte access to bit 7..0. */
        volatile uint8_t B15_8;     /**< Byte access to bit 15..8. */
        volatile uint8_t B23_16;    /**< Byte access to bit 23..16. */
        volatile uint8_t B31_24;    /**< Byte access to bit 31..24. */
        volatile uint8_t B39_32;    /**< Byte access to bit 39..32. */
        volatile uint8_t B47_40;    /**< Byte access to bit 47..40. */
        volatile uint8_t B55_48;    /**< Byte access to bit 55..48. */
        volatile uint8_t B63_56;    /**< Byte access to bit 63..56. */
    } BYTE;
    struct {
        volatile uint8_t B15_0;     /**< Halfword access to bit 15..0. */
        volatile uint8_t B31_16;    /**< Halfword access to bit 31..16. */
        volatile uint8_t B47_32;    /**< Halfword access to bit 47..32. */
        volatile uint8_t B63_48;    /**< Halfword access to bit 63..48. */
    } HWORD;
    struct {
        volatile uint8_t B31_0;     /**< Word access to bit 31..0. */
        volatile uint8_t B63_32;    /**< Word access to bit 63..32. */
    } WORD;
    volatile uint64_t DWORD;        /**< Doubleword access to all bits. */
} reg_ct_lcd_bin_t;


/**
 *  \struct reg_ct_lcd_t
 *  \brief  Representation of GPIO register.
 *
 *  Described in ct board manual p.?ff.
 */
typedef struct {
    reg_ct_lcd_i2c_t I2C;       /**< I2C interface. */
    reg_ct_lcd_bg_t BG;         /**< Background interface. */
    volatile uint16_t ASCII;    /**< ASCII interface. */
    uint8_t RESERVED[14];
    reg_ct_lcd_bin_t BIN;       /**< BINary interface. */
} reg_ct_lcd_t;


#define CT_LCD      ((reg_ct_lcd_t *) 0x60000300)


/*  GPIO register
 * ------------------------------------------------------------------------- */

/**
 *  \union  reg_ct_gpio_port_t
 *  \brief  Representation of GPIO port.
 */
typedef union {
    struct {
        volatile uint8_t P1;        /**< Byte acces to port P1. */
        volatile uint8_t P2;        /**< Byte acces to port P2. */
        volatile uint8_t P3;        /**< Byte acces to port P3. */
        volatile uint8_t P4;        /**< Byte acces to port P4. */
    } BYTE;
    struct {
        volatile uint16_t P2_1;     /**< Halfword acces to port P2/1. */
        volatile uint16_t P4_3;     /**< Halfword acces to port P4/3. */
    } HWORD;
    volatile uint32_t WORD;         /**< Word acces to all ports. */
} reg_ct_gpio_port_t;
	

/**
 *  \struct reg_ct_gpio_t
 *  \brief  Representation of GPIO register.
 *
 *  Described in ct board manual p.?ff.
 */
typedef struct {
    reg_ct_gpio_port_t OUT;     /**< OUTput. */
    uint8_t RESERVED[12];
    reg_ct_gpio_port_t IN;      /**< INput. */
} reg_ct_gpio_t;


#define CT_GPIO     ((reg_ct_gpio_t *) 0x60000400)
 
#endif
