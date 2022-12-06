#pragma once
#include "main.h"

class Tests
{
    public:
        static void run(SPI_HandleTypeDef h, uint8_t *leds);
        static void blinkLeds(SPI_HandleTypeDef h, uint8_t *leds);
        static void testStates(SPI_HandleTypeDef h);
        static void testButtons(SPI_HandleTypeDef h);
};
