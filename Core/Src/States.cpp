#include "States.h"
#include "UpdateLed.h"

States::States()
{}
/* States::States(uint8_t[] s1, uint8_t[] s2, uint8_t[] s3) */
    /* :state1(s1) */ 
/* {} */

 /* uint8_t States::current_state[] = {0x0C, 0x09, 0x0C}; */
 uint8_t States::state1[] = {0x0C, 0x09, 0x0C};
 uint8_t States::state2[] = {0x21, 0x0C, 0x09};
 uint8_t States::state3[] = {0x09, 0x09, 0x11};
 uint8_t States::state4[] = {0x09, 0x11, 0x09};
 uint8_t States::stateYellow[] = {0x12, 0x02, 0x02};

void States::blinkYellow(SPI_HandleTypeDef h)
{
    uint8_t blink[3];
    blink[2] = States::state1[2] & 0x18;
    blink[2] = blink[2] | 0x02; 

    blink[1] = States::state1[1] & 0x18;
    blink[1] = blink[2] | 0x02;

    blink[0] = 0x12;
    /* blink[0] = States::state1[0] | 0x12; */
    /* blink[2] = current_state[2] || 0x00; */
    /* blink[1] = current_state[1] || 0x00; */
    /* blink[0] = current_state[0] || 0x00; */
    /* blink[0] = blink[0] && 0x02; */ 
    UpdateLed::update(h, blink);
}
void States::runState(SPI_HandleTypeDef h, uint8_t *state)
{
    *States::current_state = *state;
    UpdateLed::update(h, state);
}
