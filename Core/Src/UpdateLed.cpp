#include "UpdateLed.h"
#include "main.h"

/* void update(SPI_HandleTypeDef h, uint8_t word1, uint8_t word2, uint8_t word3) */
void UpdateLed::update(SPI_HandleTypeDef h, uint8_t *leds)
{
    /* uint8_t spiData[3] = {word1, word2, word3}; */
    //3. reset shift register
    HAL_GPIO_WritePin(SHIFT_REG_RESET_GPIO_Port, SHIFT_REG_RESET_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SHIFT_REG_RESET_GPIO_Port, SHIFT_REG_RESET_Pin, GPIO_PIN_SET);
    //1. send 2 words

    /* uint8_t led[3] = {0x00, 0x09, 0x0C}; */
    /* uint8_t dled[3] = {0x00, 0x00, 0x00}; */
    /* uint8_t led[3] = {0x00, 0x08, 0x00}; */
    /* HAL_SPI_Transmit(&h, leds, 3, 100); */
    HAL_SPI_Transmit(&h, leds, 3, 1000);

    //2. stcp-register high
    HAL_GPIO_WritePin(SHIFT_REG_STORE_CLK_GPIO_Port, SHIFT_REG_STORE_CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SHIFT_REG_STORE_CLK_GPIO_Port, SHIFT_REG_STORE_CLK_Pin, GPIO_PIN_RESET);

    //3. reset shift register
    HAL_GPIO_WritePin(SHIFT_REG_RESET_GPIO_Port, SHIFT_REG_RESET_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SHIFT_REG_RESET_GPIO_Port, SHIFT_REG_RESET_Pin, GPIO_PIN_SET);
}
