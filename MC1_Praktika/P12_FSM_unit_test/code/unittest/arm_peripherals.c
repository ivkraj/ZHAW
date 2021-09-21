#include <stdint.h>
#include <stdlib.h>

// memory for arm peripherals & co.
int8_t apb1[ 0x7FFFF ] 
        __attribute__ ((section (".apb_1_periph")))
        __attribute__ ((__used__));

int8_t apb2[ 0x6BFF ] 
        __attribute__ ((section (".apb_2_periph")))
        __attribute__ ((__used__));
        
int8_t ahb1[ 0x5FFFF ] 
        __attribute__ ((section (".ahb_1_periph")))
        __attribute__ ((__used__));

int8_t ahb2[ 0x60BFF ] 
        __attribute__ ((section (".ahb_2_periph")))
        __attribute__ ((__used__));

int8_t intperiph[ 0xFFFFF ]
        __attribute__ ((section (".int_periph")))
        __attribute__ ((__used__));
        
int8_t fmc[ 0xFFF ]
        __attribute__ ((section (".fmc")))
        __attribute__ ((__used__));
