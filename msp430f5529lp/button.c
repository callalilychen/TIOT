#include "button.h"
struct btstate b1state = {0};
struct btstate b2state = {0};

uint16_t wdt_state = 0;
uint16_t double_count = 0xffff;
uint16_t long_count = 0xffff;

inline int release(struct btstate *);
inline int press(struct btstate *);
inline int pressed(struct btstate *);

void setupButton(uint16_t count, uint16_t lcount)
{
	P1DIR &= ~BIT1;					// Set P1.1 to input direction Button
  P1REN |= BIT1;          // with pullup resistor
  P1OUT |= BIT1;          // with pullup resistor
	P2DIR &= ~BIT1;					// Set P2.1 to input direction Button
  P2REN |= BIT1;          // with pullup resistor
  P2OUT |= BIT1;          // with pullup resistor
  double_count = count;
  long_count = lcount;
}
inline int update(struct btstate *state, int counter, uint8_t pin){
  int res = 0;
    if ( BIT1 & (state->pin & ~pin) ) {
      state->press = counter;
      state->pressed =1;
      state->released =0;
      if(state->expired){
        state->type=1;
      }else{
        state->type++;
      }
      res++;
    }
    else if ( BIT1 & (~state->pin & pin) ) {
      state->release = counter;
      state->pressed =0;
      state->released =1;
      if(state-> type == 0xff){
        state->expired = 1;
      } else{
        state->expired = 0;
      }
      res++;
    } 
    else if ( BIT1 & (state->pin & pin) ) {
      if((uint16_t)(counter - state->release) >= double_count){
        state->expired = 1;
        res++;
      }
    }
    else if ( BIT1 & ~(state->pin | pin) ) {
      if((uint16_t)(counter - state->press) >= long_count){
        state->type = 0xff;
        res++;
      }
    }
    state->pin = pin;
    return res;
}

int updateState()
{
  ENTER_MUTEXT
  int counter = wdt_counter;
  EXIT_MUTEX
  if(wdt_state != counter){
    uint8_t pin1 = P1IN;
    uint8_t pin2 = P2IN;
    wdt_state = counter;
    //print("now p1 = %x p2=%x \n", b1state.pressed, b2state.pressed);
    return update(&b1state, counter, pin1) + update(&b2state, counter, pin2);
  }else{
    __delay_cycles(10);
    return 0;
  }
}

int b1press(){
  return press(&b1state);
}
int b2press(){
  return press(&b2state);
}

int b1pressed(){
  return pressed(&b1state);
}
int b2pressed(){
  return pressed(&b2state);
}

int b1release(){
  return release(&b1state);
}
int b2release(){
  return release(&b2state);
}

inline int release(struct btstate *state){
  if(state->released){
    state->released =0;
    return 1;
  }
  return 0;
}
inline int press(struct btstate *state){
  if(state->pressed){
    state->pressed = 0;
    return 1;
  }
  return 0;
}
inline int pressed(struct btstate *state){
  if(state->released && state->expired){
    state->pressed = 0;
    state->released =0;
    return state->type;
  }
  return 0;
}
