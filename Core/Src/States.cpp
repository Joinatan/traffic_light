#include "States.h"
#include "UpdateLed.h"

//constructors ------------------
/**Default constructor.
 */
States::States()
{}

States::States(uint8_t* states)
{
    this->state = states;
}

/**Constructor
 * takes a state as uint8_t array and a delay time in ms.
 */
States::States(uint8_t* states, uint32_t delay)
{
    this->state = states;
    this->delay = delay;
}
/**Default destructor.
 */
States::~States()
{}
//-------------------------------

//------------different states
/**
 * Car lanes 1 and 3 green.
 */
uint8_t States::state1[] = {0x0C, 0x09, 0x0C};
/**
 * Car lanes 2 and 4 green.
 */
uint8_t States::state2[] = {0x21, 0x0C, 0x09};
/**
 * Car lanes 2 and 4 green.
 */
uint8_t States::state3[] = {0x09, 0x09, 0x11};
uint8_t States::state4[] = {0x09, 0x11, 0x09};
/**
 * State for both car lanes yellow.
 */
uint8_t States::stateYellowBoth[] = {0x12, 0x0a, 0x0a};
/**
 * State for car lane 1 yellow.
 */
uint8_t States::stateYellow1[] = {0x0a, 0x09, 0x0a};
/**
 * State for car lane 2 yellow.
 */
uint8_t States::stateYellow2[] = {0x11, 0x0a, 0x09};
/* uint8_t States::whiteLed1 = 0; */
/* uint8_t States::whiteLed2 = 0; */

SPI_HandleTypeDef* States::spiHandle = &hspi3;

/**
 * Test function for yellow lights.
 */
void States::yellow(SPI_HandleTypeDef h)
{
    LEDS[2] = 0x0a;
    LEDS[1] = 0x0a;
    LEDS[0] = 0x12;

    UpdateLed::update(h, LEDS);
}

/**
 * Test function for toggle white(blue) lights.
 */
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

/**
 * Toggle white lights (blue) pedestrian 1.
 */
void States::toggleWhite1()
{
        LEDS[1] = LEDS[1] ^ 0x20;
        UpdateLed::update(*States::spiHandle, LEDS);
}

/**
 * Toggle white (blue) lights pedestrian 2.
 */
void States::toggleWhite2()
{
        LEDS[2] = LEDS[2] ^ 0x20;
        UpdateLed::update(*States::spiHandle, LEDS);
}

/**
 * Shut of white (blue) lights pedestrian 1.
 */
void States::shutOffWhite1()
{
        LEDS[1] = LEDS[1] & ~0x20;
        UpdateLed::update(*States::spiHandle, LEDS);
}

/**
 * Shut of white (blue) lights pedestrian 2.
 */
void States::shutOffWhite2()
{
        LEDS[2] = LEDS[2] & ~0x20;
        UpdateLed::update(*States::spiHandle, LEDS);
}

/**
 * Send state to UpdateLed function where it's sent to the LED-bytes via SPI.
 */
void States::runState(SPI_HandleTypeDef h)
{
    LEDS[0] = this->state[0];
    LEDS[1] = this->state[1] | (LEDS[1] & 0x20);
    LEDS[2] = this->state[2] | (LEDS[2] & 0x20);
    UpdateLed::update(h, LEDS);
}
