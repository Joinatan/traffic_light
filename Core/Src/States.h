#pragma once
#include "main.h"

class States{
    public:
        States();
        /* States(uint8_t s1, uint8_t s2, uint8_t s3); */

        uint8_t current_state[3];
        static  uint8_t state1[];
        static  uint8_t state2[];
        static  uint8_t state3[];
        static  uint8_t state4[];

        static  uint8_t stateYellow[];

        void blinkYellow(SPI_HandleTypeDef h);
        void runState(SPI_HandleTypeDef h, uint8_t *state);

};
