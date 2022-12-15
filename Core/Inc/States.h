#pragma once
#include "main.h"

/** Class for the different states.
 * Every state corresponds to a States-object.
 * Also includes some static methods to control led like toggling white (blue) lights.
 * The runState-method must be called to actually update the leds.
 */
class States{
    public:
        States();
        States(uint8_t* state);
        States(uint8_t* state, uint32_t delay);

        ~States();


       /**
        * Each object has an associated state.
        */
        const uint8_t* state;
       /**
        * Duration of state. Used for RTOS-timers.
        */
        uint32_t delay;

       /**
        * Tells wheter the state is in RTOS-queue or not.
        */
        bool in_queue = false;

        /* uint8_t current_state[3]; */

        //different states for leds
        static  uint8_t state1[];
        static  uint8_t state2[];
        static  uint8_t state3[];
        static  uint8_t state4[];

        static  uint8_t stateYellowBoth[];
        static  uint8_t stateYellow1[];
        static  uint8_t stateYellow2[];

/**
 * Handle to SPI interface communicating with shift registers.
 */
        static SPI_HandleTypeDef* spiHandle;


        static void yellow(SPI_HandleTypeDef h);
        //function for the white (blue) lights
        static void toggleWhite1();
        static void toggleWhite2();
        static void shutOffWhite1();
        static void shutOffWhite2();

        //test fuction
        static void testToggleWhite();
        
        //update leds
        void runState(SPI_HandleTypeDef h);

        static uint8_t whiteLed1;
        static uint8_t whiteLed2;
};


