#include "board_ext.h"

extern uint16_t rs_value;

void setupLPM(uint16_t v);
inline void __attribute__((alwalys_inline))enterLPM(){
 __bis_SR_register(rs_value); // Enter LPM, interrupts enabled
}

