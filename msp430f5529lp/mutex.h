#include <msp430.h>
#define ENTER_MUTEXT uint16_t srGIEstate = __get_SR_register() & GIE;\
    __bic_SR_register(GIE);
 
#define EXIT_MUTEX __bis_SR_register(srGIEstate);
