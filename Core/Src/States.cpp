#include "States.h"
/* #include "main.h" */
#include "UpdateLed.h"

States::States()
{}

States::States(uint8_t* states)
{
    this->state = states;
}

States::States(uint8_t* states, uint32_t delay)
{
    this->state = states;
    this->delay = delay;
}
/* States::States(uint8_t[] s1, uint8_t[] s2, uint8_t[] s3) */
    /* :state1(s1) */ 
/* {} */

 /* uint8_t States::current_state[] = {0x0C, 0x09, 0x0C}; */
uint8_t States::state1[] = {0x0C, 0x09, 0x0C};
uint8_t States::state2[] = {0x21, 0x0C, 0x09};
uint8_t States::state3[] = {0x09, 0x09, 0x11};
uint8_t States::state4[] = {0x09, 0x11, 0x09};
uint8_t States::stateYellowBoth[] = {0x12, 0x0a, 0x0a};
uint8_t States::stateYellow1[] = {0x0a, 0x09, 0x0a};
uint8_t States::stateYellow2[] = {0x11, 0x0a, 0x09};
uint8_t States::whiteLed1 = 0;
uint8_t States::whiteLed2 = 0;
SPI_HandleTypeDef* States::spiHandle = &hspi3;

void States::yellow(SPI_HandleTypeDef h)
{
    LEDS[2] = 0x0a;
    LEDS[1] = 0x0a;
    LEDS[0] = 0x12;

    UpdateLed::update(h, LEDS);

}

void States::testToggleWhite()
{

    for(int i = 0; i < 5; i++)
    {
    
        LEDS[2] = LEDS[2] ^ 0x20;
        LEDS[1] = LEDS[1] ^ 0x20;
        UpdateLed::update(*States::spiHandle, LEDS);
        HAL_Delay(500);
    }
}

void States::toggleWhite1()
{
        /* LEDS[2] = LEDS[2] ^ 0x20; */
        LEDS[1] = LEDS[1] ^ 0x20;
        UpdateLed::update(*States::spiHandle, LEDS);
}

void States::toggleWhite2()
{
        LEDS[2] = LEDS[2] ^ 0x20;
        /* LEDS[1] = LEDS[1] ^ 0x20; */
        UpdateLed::update(*States::spiHandle, LEDS);
}

void States::shutOffWhite1()
{
        /* LEDS[2] = LEDS[2] & ~0x20; */
        LEDS[1] = LEDS[1] & ~0x20;
        UpdateLed::update(*States::spiHandle, LEDS);
}

void States::shutOffWhite2()
{
        LEDS[2] = LEDS[2] & ~0x20;
        /* LEDS[1] = LEDS[1] & ~0x20; */
        UpdateLed::update(*States::spiHandle, LEDS);
}

void States::runState(SPI_HandleTypeDef h)
{
    /* *States::current_state = *state; */
    LEDS[0] = this->state[0];
    LEDS[1] = this->state[1] | (LEDS[1] & 0x20);
    LEDS[2] = this->state[2] | (LEDS[2] & 0x20);
    UpdateLed::update(h, LEDS);
}
