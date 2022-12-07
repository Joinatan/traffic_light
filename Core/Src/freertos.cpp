/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "States.h"
#include "gpio.h"
#include <vector>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t greenDelay = 4000;
uint32_t toggleFreq = 400;
uint32_t redDelayMax = 1000;
uint32_t pedestrianDelay = 500;
uint32_t orangeDelay = 500;
uint32_t walkingDelay = 8000;

States* state_1 = new States(States::state1, greenDelay);
States* state_2 = new States(States::state2, greenDelay);
/* States* state_1 = new States(States::state1, 10); */
/* States* state_2 = new States(States::state2, 10); */
States* state_3 = new States(States::state3, walkingDelay);
States* state_4 = new States(States::state4, walkingDelay);
States* state_yellow1 = new States(States::stateYellow1, orangeDelay);
States* state_yellow2 = new States(States::stateYellow2, orangeDelay);
States* state_yellowBoth = new States(States::stateYellowBoth, orangeDelay);

States* msgBuf;

States* current_state;
States* previous_state;
States* base_states[4] = {state_yellowBoth, state_1, state_yellowBoth, state_2};
/* States* state_dummy_queue[6]; */
/* std::vector<States*> state_dummy_queue; */

volatile uint16_t gpio = 0;

/* bool carLane1 = false; */
/* bool carLane2 = false; */
bool pd1Waiting = false;
bool pd2Waiting = false;

volatile uint8_t carLane = 0x00;
uint8_t prio = 1;
uint8_t i = 1;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UPDATE_STATE_TA */
osThreadId_t UPDATE_STATE_TAHandle;
const osThreadAttr_t UPDATE_STATE_TA_attributes = {
    .name = "UPDATE_STATE_TA",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for YELLOW_TASK */
osThreadId_t YELLOW_TASKHandle;
const osThreadAttr_t YELLOW_TASK_attributes = {
    .name = "YELLOW_TASK",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for RUN_STATES_TASK */
osThreadId_t RUN_STATES_TASKHandle;
const osThreadAttr_t RUN_STATES_TASK_attributes = {
    .name = "RUN_STATES_TASK",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for PEDESTRIAN_TASK */
osThreadId_t PEDESTRIAN_TASKHandle;
const osThreadAttr_t PEDESTRIAN_TASK_attributes = {
    .name = "PEDESTRIAN_TASK",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for ISR_task */
osThreadId_t ISR_taskHandle;
const osThreadAttr_t ISR_task_attributes = {
    .name = "ISR_task",
    .stack_size = 8000 * 4,
    .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for PD_TASK2 */
osThreadId_t PD_TASK2Handle;
const osThreadAttr_t PD_TASK2_attributes = {
    .name = "PD_TASK2",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for BASE_TASK */
osThreadId_t BASE_TASKHandle;
const osThreadAttr_t BASE_TASK_attributes = {
    .name = "BASE_TASK",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for STATE_QUEUE */
osMessageQueueId_t STATE_QUEUEHandle;
const osMessageQueueAttr_t STATE_QUEUE_attributes = {
    .name = "STATE_QUEUE"
};
/* Definitions for TIM_orange_delay */
osTimerId_t TIM_orange_delayHandle;
const osTimerAttr_t TIM_orange_delay_attributes = {
    .name = "TIM_orange_delay"
};
/* Definitions for TIM_pedestrianDelay */
osTimerId_t TIM_pedestrianDelayHandle;
const osTimerAttr_t TIM_pedestrianDelay_attributes = {
    .name = "TIM_pedestrianDelay"
};
/* Definitions for TIM_green_delay */
osTimerId_t TIM_green_delayHandle;
const osTimerAttr_t TIM_green_delay_attributes = {
    .name = "TIM_green_delay"
};
/* Definitions for TIM_yellow */
osTimerId_t TIM_yellowHandle;
const osTimerAttr_t TIM_yellow_attributes = {
    .name = "TIM_yellow"
};
/* Definitions for STATE_MUTEX */
osMutexId_t STATE_MUTEXHandle;
const osMutexAttr_t STATE_MUTEX_attributes = {
    .name = "STATE_MUTEX"
};
/* Definitions for ISR_semaphore */
osSemaphoreId_t ISR_semaphoreHandle;
const osSemaphoreAttr_t ISR_semaphore_attributes = {
    .name = "ISR_semaphore"
};
/* Definitions for blink1_semaphore */
osSemaphoreId_t blink1_semaphoreHandle;
const osSemaphoreAttr_t blink1_semaphore_attributes = {
    .name = "blink1_semaphore"
};
/* Definitions for blink2_semaphore */
osSemaphoreId_t blink2_semaphoreHandle;
const osSemaphoreAttr_t blink2_semaphore_attributes = {
    .name = "blink2_semaphore"
};
/* Definitions for yellow_semaphore */
osSemaphoreId_t yellow_semaphoreHandle;
const osSemaphoreAttr_t yellow_semaphore_attributes = {
    .name = "yellow_semaphore"
};
/* Definitions for start_yellow_semaphore */
osSemaphoreId_t start_yellow_semaphoreHandle;
const osSemaphoreAttr_t start_yellow_semaphore_attributes = {
    .name = "start_yellow_semaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void UPDATE_STATE(void *argument);
void RUN_YELLOW(void *argument);
void RUN_STATES(void *argument);
void PEDESTRIAN_BLINK(void *argument);
void ISR_rtos(void *argument);
void PEDESTRIAN_BLINK2(void *argument);
void BASE(void *argument);
void Orange_delay(void *argument);
void pedestrian_delay(void *argument);
void green_delay(void *argument);
void yellow_delay(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */
    /* Create the mutex(es) */
    /* creation of STATE_MUTEX */
    STATE_MUTEXHandle = osMutexNew(&STATE_MUTEX_attributes);

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* Create the semaphores(s) */
    /* creation of ISR_semaphore */
    ISR_semaphoreHandle = osSemaphoreNew(1, 1, &ISR_semaphore_attributes);

    /* creation of blink1_semaphore */
    blink1_semaphoreHandle = osSemaphoreNew(1, 1, &blink1_semaphore_attributes);

    /* creation of blink2_semaphore */
    blink2_semaphoreHandle = osSemaphoreNew(1, 1, &blink2_semaphore_attributes);

    /* creation of yellow_semaphore */
    yellow_semaphoreHandle = osSemaphoreNew(1, 1, &yellow_semaphore_attributes);

    /* creation of start_yellow_semaphore */
    start_yellow_semaphoreHandle = osSemaphoreNew(1, 1, &start_yellow_semaphore_attributes);

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    //take yellow semaphore right away
    osSemaphoreAcquire(yellow_semaphoreHandle, osWaitForever);
    osSemaphoreAcquire(start_yellow_semaphoreHandle, osWaitForever);
    osSemaphoreAcquire(ISR_semaphoreHandle, osWaitForever);
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* Create the timer(s) */
    /* creation of TIM_orange_delay */
    TIM_orange_delayHandle = osTimerNew(Orange_delay, osTimerOnce, NULL, &TIM_orange_delay_attributes);

    /* creation of TIM_pedestrianDelay */
    TIM_pedestrianDelayHandle = osTimerNew(pedestrian_delay, osTimerOnce, NULL, &TIM_pedestrianDelay_attributes);

    /* creation of TIM_green_delay */
    TIM_green_delayHandle = osTimerNew(green_delay, osTimerPeriodic, NULL, &TIM_green_delay_attributes);

    /* creation of TIM_yellow */
    TIM_yellowHandle = osTimerNew(yellow_delay, osTimerOnce, NULL, &TIM_yellow_attributes);

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the queue(s) */
    /* creation of STATE_QUEUE */
    STATE_QUEUEHandle = osMessageQueueNew (16, sizeof(uint32_t), &STATE_QUEUE_attributes);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of UPDATE_STATE_TA */
    UPDATE_STATE_TAHandle = osThreadNew(UPDATE_STATE, NULL, &UPDATE_STATE_TA_attributes);

    /* creation of YELLOW_TASK */
    YELLOW_TASKHandle = osThreadNew(RUN_YELLOW, NULL, &YELLOW_TASK_attributes);

    /* creation of RUN_STATES_TASK */
    RUN_STATES_TASKHandle = osThreadNew(RUN_STATES, NULL, &RUN_STATES_TASK_attributes);

    /* creation of PEDESTRIAN_TASK */
    PEDESTRIAN_TASKHandle = osThreadNew(PEDESTRIAN_BLINK, NULL, &PEDESTRIAN_TASK_attributes);

    /* creation of ISR_task */
    ISR_taskHandle = osThreadNew(ISR_rtos, NULL, &ISR_task_attributes);

    /* creation of PD_TASK2 */
    PD_TASK2Handle = osThreadNew(PEDESTRIAN_BLINK2, NULL, &PD_TASK2_attributes);

    /* creation of BASE_TASK */
    BASE_TASKHandle = osThreadNew(BASE, NULL, &BASE_TASK_attributes);

    /* USER CODE BEGIN RTOS_THREADS */


    /* add threads, ... */
    osThreadTerminate(PEDESTRIAN_TASKHandle);
    osThreadTerminate(PD_TASK2Handle);
    /* osThreadTerminate(YELLOW_TASKHandle); */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* TickType_t xLastWakeTime; */
    /* const TickType_t xPeriod = pdMS_TO_TICKS(150) ; // ms to ticks */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
        /* vTaskDelayUntil( &xLastWakeTime, xPeriod ); */
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_UPDATE_STATE */
/**
 * @brief Function implementing the UPDATE_STATE_TA thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_UPDATE_STATE */
void UPDATE_STATE(void *argument)
{
    /* USER CODE BEGIN UPDATE_STATE */

    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(250) ; // ms to ticks

    xLastWakeTime = xTaskGetTickCount();

    /* Infinite loop */
    for(;;)
    {
        //------------TAKE SEMAPHORE
        osSemaphoreAcquire(blink2_semaphoreHandle, osWaitForever);
        if(current_state == state_3)
        {
                pd1Waiting = false;
        }
        if(current_state == state_4)
        {
                pd2Waiting = false;
        }
        if(osMessageQueueGetCount(STATE_QUEUEHandle) > 0)
        {
            //terminate base state
            osThreadTerminate(BASE_TASKHandle);

            i = 5;
            osMessageQueueGet(STATE_QUEUEHandle, &msgBuf, &prio, osWaitForever);

            previous_state = current_state;

            //give semaphore YELLOW_TASK and wait for semaphore
            osSemaphoreRelease(start_yellow_semaphoreHandle);
            osSemaphoreAcquire(yellow_semaphoreHandle, osWaitForever);

            current_state = msgBuf;
            osTimerStart(TIM_orange_delayHandle, current_state->delay);

            //===================test
            previous_state = current_state;

            current_state->in_queue = false;

            if(current_state == state_3)
            {
                osThreadTerminate(PEDESTRIAN_TASKHandle);
                States::shutOffWhite2();
                /* pd1Waiting = false; */
            }

            if(current_state == state_4)
            {
                osThreadTerminate(PD_TASK2Handle);
                States::shutOffWhite1();
                /* pd2Waiting = false; */
            }
        }else{
            if(osThreadGetState(BASE_TASKHandle) == osThreadTerminated)
            {
                BASE_TASKHandle = osThreadNew(BASE, NULL, &BASE_TASK_attributes);
            }
        }

        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    }
    /* USER CODE END UPDATE_STATE */
}

/* USER CODE BEGIN Header_RUN_YELLOW */
/**
 * @brief Function implementing the YELLOW_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RUN_YELLOW */
void RUN_YELLOW(void *argument)
{
    /* USER CODE BEGIN RUN_YELLOW */
    /* TickType_t xLastWakeTime; */
    /* xLastWakeTime = xTaskGetTickCount(); */
    /* const TickType_t xPeriod = pdMS_TO_TICKS(1000) ; // ms to ticks */

    /* Infinite loop */
    for(;;)
    {
        osSemaphoreAcquire(start_yellow_semaphoreHandle, osWaitForever);

        //from state_1 to any pd
        if(previous_state == state_1 && (msgBuf == state_3 || msgBuf == state_4))
        {
            current_state = state_yellow1;
        }
        //from state_1 to state_2
        if(previous_state == state_1 && msgBuf == state_2)
        {
            current_state = state_yellowBoth;
        }
        //from state_2 to any pd
        if(previous_state == state_2 && (msgBuf == state_3 || msgBuf == state_4 || msgBuf == state_1))
        {
            current_state = state_yellow2;
        }
        //from state_2 to state_1
        if(previous_state == state_2 && msgBuf == state_1)
        {
            current_state = state_yellowBoth;
        }
        //from ped to state1
        if((previous_state == state_3 || previous_state == state_4) && msgBuf == state_1)
        {
            current_state = state_yellow1;
        }
        //from ped to state2
        if((previous_state == state_3 || previous_state == state_4) && msgBuf == state_2)
        {
            current_state = state_yellow2;
        }

        //2. start yellow timer
        osTimerStart(TIM_yellowHandle, orangeDelay);


        //3. terminate? nah..
        /* vTaskDelayUntil( &xLastWakeTime, xPeriod ); */
    }
    /* USER CODE END RUN_YELLOW */
}

/* USER CODE BEGIN Header_RUN_STATES */
/**
 * @brief Function implementing the STATES_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RUN_STATES */
void RUN_STATES(void *argument)
{
    /* USER CODE BEGIN RUN_STATES */
    current_state = state_1;
    previous_state = current_state;
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(100) ; // ms to ticks
                                                    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        current_state->runState(*States::spiHandle);
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    }

    /* USER CODE END RUN_STATES */
}

/* USER CODE BEGIN Header_PEDESTRIAN_BLINK */
osThreadId PEDESTRIAN_BLINK_ID;
/**
 * @brief Function implementing the PEDESTRIAN_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_PEDESTRIAN_BLINK */
void PEDESTRIAN_BLINK(void *argument)
{
    /* USER CODE BEGIN PEDESTRIAN_BLINK */
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(toggleFreq) ; // ms to ticks
                                                           // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    PEDESTRIAN_BLINK_ID = osThreadGetId();
    /* Infinite loop */
    for(;;)
    {
        States::toggleWhite2();
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    }
    /* USER CODE END PEDESTRIAN_BLINK */
}

/* USER CODE BEGIN Header_ISR_rtos */
/**
 * @brief Function implementing the ISR_task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ISR_rtos */
void ISR_rtos(void *argument)
{
    /* USER CODE BEGIN ISR_rtos */

    /* Infinite loop */
    for(;;)
    {
        osSemaphoreAcquire(ISR_semaphoreHandle, osWaitForever);

        switch(gpio)
        {
            case CAR1_Pin:
                /* if(HAL_GPIO_ReadPin(CAR1_GPIO_Port, CAR1_Pin) == 1 || HAL_GPIO_ReadPin(CAR3_GPIO_Port, CAR3_Pin) == 1) */
                /* if(i < 5 && carLane == 0x00) */
                if(carLane == 0x00 && pd1Waiting == false && pd2Waiting == false)
                {   
                    osSemaphoreRelease(blink2_semaphoreHandle);
                }
                if(HAL_GPIO_ReadPin(CAR1_GPIO_Port, CAR1_Pin) == 1)
                {
                    carLane = carLane | 0x01;

                    if(state_1->in_queue == false && current_state != state_1)
                    {
                        /* osMessageQueuePut(STATE_QUEUEHandle, &state_yellowBoth, 0, osWaitForever); */
                        /* state_dummy_queue.put() */

                        osMessageQueuePut(STATE_QUEUEHandle, &state_1, 0, osWaitForever);
                        state_1->in_queue = true;

                    }

                }else{
                    if(HAL_GPIO_ReadPin(CAR3_GPIO_Port, CAR3_Pin) == 0)
                    {
                        carLane = carLane & 0x02;
                    }
                }
                break;
            case CAR2_Pin:
                /* if(HAL_GPIO_ReadPin(CAR2_GPIO_Port, CAR2_Pin) == 1 || HAL_GPIO_ReadPin(CAR4_GPIO_Port, CAR4_Pin) == 1) */
                /* if(i < 5 && carLane == 0x00) */
                if(carLane == 0x00 && pd1Waiting == false && pd2Waiting == false)
                {   
                    osSemaphoreRelease(blink2_semaphoreHandle);
                }
                if(HAL_GPIO_ReadPin(CAR2_GPIO_Port, CAR2_Pin) == 1)
                {
                    carLane = carLane | 0x02;

                    if(state_2->in_queue == false && current_state != state_2)
                    {
                        /* osMessageQueuePut(STATE_QUEUEHandle, &state_yellowBoth, 0, osWaitForever); */
                        osMessageQueuePut(STATE_QUEUEHandle, &state_2, 0, osWaitForever);
                        state_2->in_queue = true;

                    }

                }else{
                    if(HAL_GPIO_ReadPin(CAR4_GPIO_Port, CAR4_Pin) == 0)
                    {
                        carLane = carLane & 0x01;
                    }
                }
                break;
            case CAR3_Pin:
                /* if(i < 5 && carLane == 0x00) */
                if(carLane == 0x00 && pd1Waiting == false && pd2Waiting == false)
                {   
                    osSemaphoreRelease(blink2_semaphoreHandle);
                }
                /* if(HAL_GPIO_ReadPin(CAR1_GPIO_Port, CAR1_Pin) == 1 || HAL_GPIO_ReadPin(CAR3_GPIO_Port, CAR3_Pin) == 1) */
                if(HAL_GPIO_ReadPin(CAR3_GPIO_Port, CAR3_Pin) == 1)
                {
                    carLane = carLane | 0x01;

                    if(state_1->in_queue == false && current_state != state_1)
                    {
                        /* osMessageQueuePut(STATE_QUEUEHandle, &state_yellowBoth, 0, osWaitForever); */
                        osMessageQueuePut(STATE_QUEUEHandle, &state_1, 0, osWaitForever);
                        state_1->in_queue = true;

                    }

                }else{
                    if(HAL_GPIO_ReadPin(CAR1_GPIO_Port, CAR1_Pin) == 0)
                    {
                        carLane = carLane & 0x02;
                    }
                }
                break;
            case CAR4_Pin:
                /* if(i < 5 && carLane == 0x00) */
                if(carLane == 0x00 && pd1Waiting == false && pd2Waiting == false)
                {   
                    osSemaphoreRelease(blink2_semaphoreHandle);
                }
                /* if(HAL_GPIO_ReadPin(CAR2_GPIO_Port, CAR2_Pin) == 1 || HAL_GPIO_ReadPin(CAR4_GPIO_Port, CAR4_Pin) == 1) */
                if(HAL_GPIO_ReadPin(CAR4_GPIO_Port, CAR4_Pin) == 1)
                {
                    carLane = carLane | 0x02;

                    if(state_2->in_queue == false && current_state != state_2)
                    {
                        /* osMessageQueuePut(STATE_QUEUEHandle, &state_yellowBoth, 0, osWaitForever); */
                        osMessageQueuePut(STATE_QUEUEHandle, &state_2, 0, osWaitForever);
                        state_2->in_queue = true;
                    }
                }else{
                    if(HAL_GPIO_ReadPin(CAR2_GPIO_Port, CAR2_Pin) == 0)
                    {
                        carLane = carLane & 0x01;
                    }
                }
                break;
            case PEDESTRIAN1_Pin:
                if(state_3->in_queue == false && current_state != state_3)
                {
                    /* if(osMessageQueueGetCount(STATE_QUEUEHandle) < 1 && carLane == 0x00) */
                    if(!pd2Waiting && !pd1Waiting && carLane == 0x00)
                    {
                        osSemaphoreRelease(blink2_semaphoreHandle);
                    }
                    osMessageQueuePut(STATE_QUEUEHandle, &state_3, 1, osWaitForever);
                    state_3->in_queue = true;
                    pd1Waiting = true;

                    osTimerStart(TIM_pedestrianDelayHandle, pedestrianDelay);
                    PEDESTRIAN_TASKHandle = osThreadNew(PEDESTRIAN_BLINK, NULL, &PEDESTRIAN_TASK_attributes);
                }
                break;
            case PEDESTRIAN2_Pin:
                if(state_4->in_queue == false && current_state != state_4)
                {
                    /* if(osMessageQueueGetCount(STATE_QUEUEHandle) == 0 && carLane == 0x00) */
                    if(!pd2Waiting && !pd1Waiting && carLane == 0x00)
                    {
                        osSemaphoreRelease(blink2_semaphoreHandle);
                    }
                    osMessageQueuePut(STATE_QUEUEHandle, &state_4, 1, osWaitForever);
                    state_4->in_queue = true;
                    pd2Waiting = true;

                    osTimerStart(TIM_pedestrianDelayHandle, pedestrianDelay);
                    PD_TASK2Handle = osThreadNew(PEDESTRIAN_BLINK2, NULL, &PD_TASK2_attributes);
                }
                break;
        }

    }
    /* USER CODE END ISR_rtos */
}

/* USER CODE BEGIN Header_PEDESTRIAN_BLINK2 */
/**
 * @brief Function implementing the PD_TASK2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_PEDESTRIAN_BLINK2 */
void PEDESTRIAN_BLINK2(void *argument)
{
    /* USER CODE BEGIN PEDESTRIAN_BLINK2 */
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(toggleFreq) ; // ms to ticks
                                                           // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    /* PEDESTRIAN_BLINK_ID = osThreadGetId(); */
    /* Infinite loop */
    for(;;)
    {
        States::toggleWhite1();
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    }
    /* USER CODE END PEDESTRIAN_BLINK2 */
}

/* USER CODE BEGIN Header_BASE */
/**
 * @brief Function implementing the BASE_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_BASE */
void BASE(void *argument)
{
    /* USER CODE BEGIN BASE */
    TickType_t xLastWakeTime;
    /* TickType_t xPeriod = pdMS_TO_TICKS(200) ; // ms to ticks */
    TickType_t xPeriod;
    xLastWakeTime = xTaskGetTickCount();

    /* Infinite loop */
    for(;;)
    {
        switch(carLane)
        {
            case 0x01:
                if(previous_state == state_1)
                {
                    current_state = state_1;
                    previous_state = current_state;
                    i = 1;
                }else{
                    current_state = state_yellow1;
                    previous_state = state_1;
                    i = 0;
                }
                break;

            case 0x02:
                if(previous_state == state_2)
                {
                    current_state = state_2;
                    previous_state = current_state;
                    i = 3;
                }else{
                    current_state = state_yellow2;
                    previous_state = state_2;
                    i = 2;
                }
                break;
            default:
                if(i == 5)
                {
                    if(previous_state == state_3 || previous_state == state_4)
                    {
                        current_state = state_yellow1;
                        i = 1;
                    }
                    if(previous_state == state_1)
                    {
                        current_state = state_yellowBoth;
                        i = 3;
                    }
                    if(previous_state == state_2)
                    {
                        current_state = state_yellowBoth;
                        i = 1;
                    }
                }else{
                    current_state = base_states[i];
                    previous_state = current_state;
                    i = (i+1)%4;
                }

                break;
        }

        xPeriod = pdMS_TO_TICKS(current_state->delay);
        osTimerStart(TIM_orange_delayHandle, current_state->delay * 0.8);

        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    } 
    /* USER CODE END BASE */
}

/* Orange_delay function */
void Orange_delay(void *argument)
{
    /* USER CODE BEGIN Orange_delay */
    osSemaphoreRelease(blink2_semaphoreHandle);
    /* USER CODE END Orange_delay */
}

/* pedestrian_delay function */
void pedestrian_delay(void *argument)
{
    /* USER CODE BEGIN pedestrian_delay */
    //osThreadTerminate(PEDESTRIAN_BLINK_ID);
    //current_state = state_3;
    //
    /* osThreadTerminate(PEDESTRIAN_TASKHandle); */
    /* States::shutOffWhite1(); */

    /* USER CODE END pedestrian_delay */
}

/* green_delay function */
void green_delay(void *argument)
{
    /* USER CODE BEGIN green_delay */
    current_state = base_states[i];
    i = (i+1)%4;
    /* USER CODE END green_delay */
}

/* yellow_delay function */
void yellow_delay(void *argument)
{
    /* USER CODE BEGIN yellow_delay */
    osSemaphoreRelease(yellow_semaphoreHandle);
    /* USER CODE END yellow_delay */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE BEGIN 1 */
#ifndef RUN_TEST_PROGRAM
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    gpio = GPIO_Pin;
    osSemaphoreRelease(ISR_semaphoreHandle);
    //xSemaphoreGiveFromISR(ISR_semaphoreHandle);
}
#endif
/* USER CODE END 1 */

/* USER CODE END Application */

