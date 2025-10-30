/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stepper.h"
#include "button.h"
#include "fnd.h"

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

/* USER CODE BEGIN PV */

enum{
  FIRSTFLOOR,
  SECONDFLOOR,
  THIRDFLOOR
};

enum{
  DOWN_START,
  UP_START
};

volatile uint8_t currFloor = 0b001;     // 현재 층수
volatile uint8_t targetFloor = 0b000;   // 예약 층수
volatile uint32_t tickCount;        // 엘리베이터 도착한 후 다음 동작 대기시간 3초
uint8_t elevatorDir;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// 포토 인터럽트 함수 : Rising Edge trigger. 엘리베이터가 해당 층 도착 시 버튼 LED 를 끄게 동작.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case GPIO_PIN_8:  // 1층 도착 시 1층 버튼 LED OFF
      currFloor = 0b001;      // 현재 층 1층으로 갱신
      FND_WriteDigit(1);      // 1층 표시
      if(led1)
      {
        led1 = !led1;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, led1);  // LED 1 OFF
        targetFloor &= ~(1 << FIRSTFLOOR);      // 1층 예약 해제
        elevator = STOP;              // 엘리베이터 동작 정지
        elevatorDir = UP_START;
        HAL_TIM_Base_Start_IT(&htim10);       // 대기 3초
      }
      else
      {
        elevatorDir = UP_START;
        if(!targetFloor)
        {
          HAL_TIM_Base_Stop_IT(&htim11);      // 예약층이 아예 없으면 정지
          elevator = STOP;
        }
        else
        {
          if(currFloor < targetFloor && elevatorDir)                  // 예약층이 위층이면 상승
          {
            elevator = UP;
          }
        }
      }
    break;

    case GPIO_PIN_6:  // 2층 도착 시 2층 버튼 LED OFF
      currFloor = 0b010;
      FND_WriteDigit(2);      // 2층 표시
      if(led2)
      {
        led2 = !led2;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, led2);  // LED 2 OFF
        targetFloor &= ~(1 << SECONDFLOOR);        // 2층 예약 해제
        elevator = STOP;
        HAL_TIM_Base_Start_IT(&htim10);
      }
      else
      {
        if(!targetFloor)
        {
          HAL_TIM_Base_Stop_IT(&htim11);
          elevator = STOP;
        }
        else
        {
          if(currFloor > targetFloor && !elevatorDir)
          {
            elevator = DOWN;
          }
          else if(currFloor == 2 && currFloor > targetFloor && !led3)
          {
            elevatorDir = DOWN_START;
            elevator = DOWN;
          }
          else if(currFloor < targetFloor && elevatorDir)
          {
            elevator = UP;
          }
          else if(currFloor == 2 && currFloor < targetFloor && !led1)
          {
            elevatorDir = UP_START;
            elevator = UP;
          }

        }
      }
    break;

    case GPIO_PIN_5:  // 3층 도착 시 3층 버튼 LED OFF
      currFloor = 0b100;
      FND_WriteDigit(3);    // 3층 표시
      if(led3)
      {
        led3 = !led3;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, led3);  // LED 3 OFF
        targetFloor &= ~(1 << THIRDFLOOR);         // 3층 예약 해제
        elevator = STOP;
        elevatorDir = DOWN_START;
        HAL_TIM_Base_Start_IT(&htim10);
      }
      else
      {
        elevatorDir = DOWN_START;
        if(!targetFloor)
        {
          HAL_TIM_Base_Stop_IT(&htim11);
          elevator = STOP;
        }
        else
        {
          if(currFloor > targetFloor && !elevatorDir)
          {
            elevator = DOWN;
          }
        }
      }
    break;

    default:
    break;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // 타이머 1ms 마다 호출 : 모터 동작
  if(htim -> Instance == TIM11)
  {
    stepMotor(motorState);
    if(motorState != 7)
    {
      motorState++;
    }
    else
    {
      motorState = 0;
    }
  }

  // 원하는 층 도착 후 문 여닫는 동작 대기 3초
  if(htim -> Instance == TIM10)
  {
    elevator = STOP;
    tickCount++;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
    BuzzerOn();
    if(tickCount >= 3000)
    {
      HAL_TIM_Base_Stop_IT(&htim10);
      __HAL_TIM_SET_COUNTER(&htim10, 0);
      tickCount = 0;
      BuzzerOff();
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    }
  }
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM11_Init();
  MX_TIM10_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  FND_WriteDigit(1);      // 초기 상태 : 1층 표시

  BuzzerInit();
  elevatorDir = UP_START;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // 층수 버튼을 눌러 LED 동작하는 함수
    buttonState(currFloor, &targetFloor, &elevator);

    if(!tickCount)    // 엘리베이터 문이 열려있지 않으면
    {
      if(elevator == STOP)  // 엘리베이터가 정지 중이면
      {
        if((currFloor > targetFloor) && targetFloor && !elevatorDir)  // 아래층 이동
        {
          elevator = DOWN;
          HAL_TIM_Base_Start_IT(&htim11);
        }
        else if((currFloor < targetFloor) && targetFloor && elevatorDir && currFloor != 4) // 순차이동 업
        {
          elevator = UP;
          HAL_TIM_Base_Start_IT(&htim11);
        }
        else if(currFloor < targetFloor && !elevatorDir && currFloor != 1 && targetFloor != 4) // 순차이동 다운
        {
          elevator = DOWN;
          HAL_TIM_Base_Start_IT(&htim11);
        }
        else if(currFloor == 2 && targetFloor == 4)  // 2층에서 3층으로
        {
          elevator = UP;
          HAL_TIM_Base_Start_IT(&htim11);
        }
        else if(currFloor == 2 && targetFloor == 1) // 2층에서 1층으로
        {
          elevator = DOWN;
          HAL_TIM_Base_Start_IT(&htim11);
        }
      }
    }
    else    // 엘베 문이 열려있으면 엘베 대기.
    {
      HAL_TIM_Base_Stop_IT(&htim11);
      __HAL_TIM_SET_COUNTER(&htim11, 0);
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
