#include "tests.h"
#include "main.h"
#include "UpdateLed.h"
#include "States.h"
/* #include <cstdint> */
/* #include <stdint.h> */

/**
 * Main test function to run different test.
 */
void Tests::run(SPI_HandleTypeDef h, uint8_t *leds)
{
    /* Tests::blinkLeds(h, leds); */
    /* Tests::testStates(h); */
    Tests::testButtons(h);
    return;
}

/**
 * Blink leds one by on in order. Tests that all are working and controlling mechanism works.
 */
void Tests::blinkLeds(SPI_HandleTypeDef h, uint8_t *leds)
{
    leds[2] = 1;
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

/**
 * Test all the different states of project. 
 */
void Tests::testStates(SPI_HandleTypeDef h)
{
States* state1 = new States(States::state1);
States* state2 = new States(States::state2);
States* state3 = new States(States::state3);
States* state4 = new States(States::state4);
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

    for(int i = 0; i < 8; i++)
    {
        States::testToggleWhite();
        HAL_Delay(500);
    }
}

/**
 * Test function thats get called from EXTI-handler when in debug mode.
 */
void Tests::testButtons(SPI_HandleTypeDef h)
{
    States* state1 = new States(States::state1);
    state1->runState(h);
    for(;;)
    {
    }
}
