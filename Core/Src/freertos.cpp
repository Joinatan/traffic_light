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
uint32_t toggleFreq = 800;
uint32_t redDelayMax = 1000;
uint32_t pedestrianDelay = 4000;
uint32_t orangeDelay = 1500;
uint32_t walkingDelay = 5000;

States* state_1 = new States(States::state1, greenDelay);
States* state_2 = new States(States::state2, greenDelay);
States* state_3 = new States(States::state3, walkingDelay);
States* state_4 = new States(States::state4, walkingDelay);
States* state_yellow = new States(States::stateYellow, orangeDelay);

States* current_state;
States* base_states[4] = {state_1, state_yellow, state_2, state_yellow};

volatile uint16_t gpio = 0;

bool carLane1 = false;
bool carLane2 = false;


uint8_t i = 0;

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
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for YELLOW_TASK */
osThreadId_t YELLOW_TASKHandle;
const osThreadAttr_t YELLOW_TASK_attributes = {
  .name = "YELLOW_TASK",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for RUN_STATES_TASK */
osThreadId_t RUN_STATES_TASKHandle;
const osThreadAttr_t RUN_STATES_TASK_attributes = {
  .name = "RUN_STATES_TASK",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for PEDESTRIAN_TASK */
osThreadId_t PEDESTRIAN_TASKHandle;
const osThreadAttr_t PEDESTRIAN_TASK_attributes = {
  .name = "PEDESTRIAN_TASK",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ISR_task */
osThreadId_t ISR_taskHandle;
const osThreadAttr_t ISR_task_attributes = {
  .name = "ISR_task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for PD_TASK2 */
osThreadId_t PD_TASK2Handle;
const osThreadAttr_t PD_TASK2_attributes = {
  .name = "PD_TASK2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
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
void Orange_delay(void *argument);
void pedestrian_delay(void *argument);
void green_delay(void *argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of TIM_orange_delay */
  TIM_orange_delayHandle = osTimerNew(Orange_delay, osTimerOnce, NULL, &TIM_orange_delay_attributes);

  /* creation of TIM_pedestrianDelay */
  TIM_pedestrianDelayHandle = osTimerNew(pedestrian_delay, osTimerOnce, NULL, &TIM_pedestrianDelay_attributes);

  /* creation of TIM_green_delay */
  TIM_green_delayHandle = osTimerNew(green_delay, osTimerPeriodic, NULL, &TIM_green_delay_attributes);

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

  /* USER CODE BEGIN RTOS_THREADS */


    /* add threads, ... */
    osThreadTerminate(PEDESTRIAN_TASKHandle);
    osThreadTerminate(PD_TASK2Handle);
    osThreadTerminate(YELLOW_TASKHandle);
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
    const TickType_t xPeriod = pdMS_TO_TICKS(150) ; // ms to ticks

    States* msgBuf;
    /* Infinite loop */
    for(;;)
    {
        //------------TAKE SEMAPHORE
        if(osMessageQueueGetCount(STATE_QUEUEHandle) > 0)
        {
            osSemaphoreAcquire(blink2_semaphoreHandle, osWaitForever);
            //osTimerStop(TIM_green_delayHandle);
            osMessageQueueGet(STATE_QUEUEHandle, &msgBuf, NULL, osWaitForever);
            current_state = msgBuf;
            osTimerStart(TIM_orange_delayHandle, current_state->delay);

            current_state->in_queue = false;

            if(current_state == state_3)
            {
                States::shutOffWhite2();
                osThreadTerminate(PEDESTRIAN_TASKHandle);
            }

            if(current_state == state_4)
            {
                States::shutOffWhite1();
                osThreadTerminate(PD_TASK2Handle);
            }


        }else{
            //start base timer
            /* if(osTimerIsRunning(TIM_green_delayHandle)==0) */
            /* { */
            /*     if(current_state == state_yellow) */
            /*     { */
            /*         osTimerStart(TIM_green_delayHandle, orangeDelay); */
            /*     }else{ */
            /*         osTimerStart(TIM_green_delayHandle, greenDelay); */
            /*     } */
            /* } */
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
    /* Infinite loop */
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        vTaskDelayUntil( &xLastWakeTime, 1000 );
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
    //osSemaphoreAcquire(blink1_semaphoreHandle, osWaitForever);
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
                if(HAL_GPIO_ReadPin(CAR1_GPIO_Port, CAR1_Pin) || HAL_GPIO_ReadPin(CAR3_GPIO_Port, CAR3_Pin))
                {
                    carLane1 = true;
                }else{
                    carLane1 = false;
                }
                if(state_1->in_queue == false && current_state != state_1)
                {
                    osMessageQueuePut(STATE_QUEUEHandle, &state_yellow, 0, osWaitForever);
                    osMessageQueuePut(STATE_QUEUEHandle, &state_1, 0, osWaitForever);
                    state_1->in_queue = true;

                }
                break;
            case CAR2_Pin:
                if(HAL_GPIO_ReadPin(CAR2_GPIO_Port, CAR2_Pin) || HAL_GPIO_ReadPin(CAR4_GPIO_Port, CAR4_Pin))
                {
                    carLane2 = true;
                }else{
                    carLane2 = false;
                }
                if(state_2->in_queue == false && current_state != state_2)
                {
                    osMessageQueuePut(STATE_QUEUEHandle, &state_yellow, 0, osWaitForever);
                    osMessageQueuePut(STATE_QUEUEHandle, &state_2, 0, osWaitForever);
                    state_2->in_queue = true;

                }
                break;
            case CAR3_Pin:
                if(HAL_GPIO_ReadPin(CAR1_GPIO_Port, CAR1_Pin) || HAL_GPIO_ReadPin(CAR3_GPIO_Port, CAR3_Pin))
                {
                    carLane1 = true;
                }else{
                    carLane1 = false;
                }
                if(state_1->in_queue == false && current_state != state_1)
                {
                    osMessageQueuePut(STATE_QUEUEHandle, &state_yellow, 0, osWaitForever);
                    osMessageQueuePut(STATE_QUEUEHandle, &state_1, 0, osWaitForever);
                    state_1->in_queue = true;

                }
                break;
            case CAR4_Pin:
                if(HAL_GPIO_ReadPin(CAR2_GPIO_Port, CAR2_Pin) || HAL_GPIO_ReadPin(CAR4_GPIO_Port, CAR4_Pin))
                {
                    carLane2 = true;
                }else{
                    carLane2 = false;
                }
                if(state_2->in_queue == false && current_state != state_2)
                {
                    osMessageQueuePut(STATE_QUEUEHandle, &state_yellow, 0, osWaitForever);
                    osMessageQueuePut(STATE_QUEUEHandle, &state_2, 0, osWaitForever);
                    state_2->in_queue = true;
                }
                break;
            case PEDESTRIAN1_Pin:
                if(state_3->in_queue == false && current_state != state_3)
                {
                    osMessageQueuePut(STATE_QUEUEHandle, &state_yellow, 0, osWaitForever);
                    osMessageQueuePut(STATE_QUEUEHandle, &state_3, 0, osWaitForever);
                    state_3->in_queue = true;

                    osTimerStart(TIM_pedestrianDelayHandle, pedestrianDelay);
                    PEDESTRIAN_TASKHandle = osThreadNew(PEDESTRIAN_BLINK, NULL, &PEDESTRIAN_TASK_attributes);
                }
                break;
            case PEDESTRIAN2_Pin:
                if(state_4->in_queue == false && current_state != state_4)
                {
                    osMessageQueuePut(STATE_QUEUEHandle, &state_yellow, 0, osWaitForever);
                    osMessageQueuePut(STATE_QUEUEHandle, &state_4, 0, osWaitForever);
                    state_4->in_queue = true;

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
    //osSemaphoreAcquire(blink1_semaphoreHandle, osWaitForever);
    /* Infinite loop */
    for(;;)
    {
        States::toggleWhite1();
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    }
  /* USER CODE END PEDESTRIAN_BLINK2 */
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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE BEGIN 1 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    gpio = GPIO_Pin;
    osSemaphoreRelease(ISR_semaphoreHandle);
    //xSemaphoreGiveFromISR(ISR_semaphoreHandle);
}
/* USER CODE END 1 */

/* USER CODE END Application */

