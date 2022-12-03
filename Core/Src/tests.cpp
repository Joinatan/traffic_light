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

void Tests::testStates(SPI_HandleTypeDef h)
{
    States states;
    /* UpdateLed::update(h, states.state1); */
    states.runState(h, states.state1);
        HAL_Delay(500 * 2);

    states.blinkYellow(h);
        HAL_Delay(500 * 2);
    /* UpdateLed::update(h, states.state2); */
    /*     HAL_Delay(500 * 2); */
    /* UpdateLed::update(h, states.state3); */
    /*     HAL_Delay(500 * 2); */
    /* UpdateLed::update(h, states.state4); */
    /*     HAL_Delay(500 * 2); */
    /* UpdateLed::update(h, states.stateYellow); */
    /*     HAL_Delay(500 * 2); */
}
