#pragma once
#include "main.h"

class States{
    public:
        States();
        States(uint8_t* state);
        States(uint8_t* state, uint32_t delay);
        /* States(uint8_t s1, uint8_t s2, uint8_t s3); */


        uint8_t* state;
        uint32_t delay;

        bool in_queue = false;

        uint8_t current_state[3];

        static  uint8_t state1[];
        static  uint8_t state2[];
        static  uint8_t state3[];
        static  uint8_t state4[];

        static  uint8_t stateYellow[];

        static SPI_HandleTypeDef* spiHandle;

        static void yellow(SPI_HandleTypeDef h);
        /* static void testToggleWhite(SPI_HandleTypeDef h); */
        static void testToggleWhite();
        static void toggleWhite1();
        static void toggleWhite2();
        static void shutOffWhite1();
        static void shutOffWhite2();
        void runState(SPI_HandleTypeDef h);

        static uint8_t whiteLed1;
        static uint8_t whiteLed2;
};


