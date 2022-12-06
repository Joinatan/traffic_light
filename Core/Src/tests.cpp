#include "tests.h"
#include "main.h"
#include "UpdateLed.h"
#include "States.h"
/* #include <cstdint> */
/* #include <stdint.h> */
void Tests::run(SPI_HandleTypeDef h, uint8_t *leds)
{
    /* Tests::blinkLeds(h, leds); */
    Tests::testStates(h);
    return;
}

void Tests::blinkLeds(SPI_HandleTypeDef h, uint8_t *leds)
{
    leds[2] = 1;
    /* int leds = 1; */
    for(int i = 0; i < 6; i++)
    {
        UpdateLed::update(h, leds);
        HAL_Delay(400);
        leds[2] *= 2;
    }
    leds[2] = 0;
    leds[1] = 1;
    for(int i = 0; i < 6; i++)
    {
        UpdateLed::update(h, leds);
        HAL_Delay(400);
        leds[1] *= 2;
    }
    leds[1] = 0;
    leds[0] = 1;
    uint8_t arr[3] = {0, 0, 0};
    for(int i = 0; i < 6; i++)
    {
        UpdateLed::update(h, leds);
        HAL_Delay(400);
        UpdateLed::update(h, arr);
        leds[0] *= 2;
    }
}

uint8_t state_1[] = {0x0C, 0x09, 0x0C};
uint8_t state_2[] = {0x21, 0x0C, 0x09};
uint8_t state_3[] = {0x09, 0x09, 0x11};
uint8_t state_4[] = {0x09, 0x11, 0x09};
void Tests::testStates(SPI_HandleTypeDef h)
{
    /* States states; */
    States* state1 = new States(state_1);
    States* state2 = new States(state_2);
    States* state3 = new States(state_3);
    States* state4 = new States(state_4);
    state1->runState(h);
        HAL_Delay(500 * 2);
        States::yellow(h);
        HAL_Delay(500 * 2);

    state2->runState(h);
        HAL_Delay(500 * 2);
        States::yellow(h);
        HAL_Delay(500 * 2);

    state3->runState(h);
        HAL_Delay(500 * 2);
        States::yellow(h);
        HAL_Delay(500 * 2);

    state4->runState(h);
        HAL_Delay(500 * 2);
        States::yellow(h);
        HAL_Delay(500 * 2);

    /* States::testToggleWhite(h); */
    for(int i = 0; i < 8; i++)
    {
        States::testToggleWhite(h);
        HAL_Delay(500);
    }
    /* UpdateLed::update(h, state1->state); */
    /* state1.state = state_1; */
    /* UpdateLed::update(h, states.state1); */
    /* states.runState(h, states.state1); */
    /*     HAL_Delay(500 * 2); */

    /* states.blinkYellow(h); */
    /*     HAL_Delay(500 * 2); */
    /* UpdateLed::update(h, states.state2); */
    /*     HAL_Delay(500 * 2); */
    /* UpdateLed::update(h, states.state3); */
    /*     HAL_Delay(500 * 2); */
    /* UpdateLed::update(h, states.state4); */
    /*     HAL_Delay(500 * 2); */
    /* UpdateLed::update(h, states.stateYellow); */
    /*     HAL_Delay(500 * 2); */
}
