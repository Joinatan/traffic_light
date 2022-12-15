#include "UpdateLed.h"
#include "main.h"

/**
 * Function to update the leds.
 * Takes SPI-handle and a pointer to bytearray to be sent tol leds.
 */
void UpdateLed::update(SPI_HandleTypeDef h, uint8_t *leds)
{
    //Send spi
    HAL_SPI_Transmit(&h, leds, 3, 1000);

    //Make the new bits appear on the outputs of shift registers. (Toggle storeclk pin) 
    HAL_GPIO_WritePin(SHIFT_REG_STORE_CLK_GPIO_Port, SHIFT_REG_STORE_CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SHIFT_REG_STORE_CLK_GPIO_Port, SHIFT_REG_STORE_CLK_Pin, GPIO_PIN_RESET);

    //Reset the shift registers. For some reason it works better when you do..
    /* HAL_GPIO_WritePin(SHIFT_REG_RESET_GPIO_Port, SHIFT_REG_RESET_Pin, GPIO_PIN_RESET); */
    /* HAL_GPIO_WritePin(SHIFT_REG_RESET_GPIO_Port, SHIFT_REG_RESET_Pin, GPIO_PIN_SET); */
}
