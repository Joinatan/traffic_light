#pragma once
/* #include <cstdint> */
#include "main.h"
/* #include "stm32l4xx_it.h" */

class UpdateLed{
    public:
        UpdateLed()
        {}

        static void update(SPI_HandleTypeDef h, uint8_t word1, uint8_t word2, uint8_t word3);
        //static void update(SPI_HandleTypeDef h, uint8_t *leds);
};
