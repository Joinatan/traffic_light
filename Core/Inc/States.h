#pragma once
#include "main.h"

class States{
    public:
        States();
        States(uint8_t* state);
        /* States(uint8_t s1, uint8_t s2, uint8_t s3); */


        uint8_t* state;

        bool in_queue = false;

        uint8_t current_state[3];

        static  uint8_t state1[];
        static  uint8_t state2[];
        static  uint8_t state3[];
        static  uint8_t state4[];

        static  uint8_t stateYellow[];

        static const SPI_HandleTypeDef spiHandle;

        static void yellow(SPI_HandleTypeDef h);
        /* static void testToggleWhite(SPI_HandleTypeDef h); */
        static void testToggleWhite();
        void runState(SPI_HandleTypeDef h);

        static uint8_t whiteLed1;
        static uint8_t whiteLed2;

};