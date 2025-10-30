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
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BCD_TO_DEC(val)  (((val >> 4) * 10) + (val & 0x0F))
#define DEC_TO_BCD(val)  (((val / 10) << 4) | (val % 10))

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

RTC_TimeTypeDef rtcTime;
RTC_DateTypeDef rtcDate;

char temp[50];
char temp2[50];

uint8_t rxData;
uint8_t buffIndex = 0;
uint8_t buff[30];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


// uart callback 함수는 단 하나
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart -> Instance == USART2)
  {

    if(rxData == '\n') // 수신된 데이터가 \n 라면
    {
      buff[buffIndex] = 0;    // 널 문자 추가해서 문자열로 만듬
      char *ptr;

      if((ptr = strstr((char *)buff, "setTime")) != NULL)
      {
        // "setTime" 명령 다음의 숫자를 BCD 형태로 변환해서 시간 설정
        rtcTime.Hours   = ((*(ptr + 7) - '0') << 4) + (*(ptr + 8) - '0');   //aski 코드값에서 '0' 이란 문자값을 빼주면 10진수 값이 된다
        rtcTime.Minutes = ((*(ptr + 9) - '0') << 4) + (*(ptr + 10) - '0');
        rtcTime.Seconds = ((*(ptr + 11) - '0') << 4) + (*(ptr + 12) - '0');

        HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD);
      }
      else if((ptr = strstr((char *)buff, "setDate")) != NULL)
      {
        // "setDate" 명령 다음의 숫자를 BCD 형태로 변환해서 날짜 설정
        rtcDate.Year  = ((*(ptr + 7) - '0') << 4) + (*(ptr + 8) - '0');    // Year 설정
        rtcDate.Month = ((*(ptr + 9) - '0') << 4) + (*(ptr + 10) - '0');  // Month 설정
        rtcDate.Date  = ((*(ptr + 11) - '0') << 4) + (*(ptr + 12) - '0');  // Date 설정

        HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD);
      }
      buffIndex = 0;
    }
    else
    {
      // 수신된 데이터가 \n 아니면
      if(buffIndex < 30)
      {
        buff[buffIndex++] = rxData;   // 버퍼에 데이터 추가
      }
    }

    HAL_UART_Receive_IT(&huart2, &rxData, 1);
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
  MX_RTC_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart2, &rxData, 1);

  i2c_lcd_init();                // LCD 한 번만 초기화

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD);

    sprintf(temp2, "\r\n20%02x-%02x-%02x %02x:%02x:%02x",
            rtcDate.Year, rtcDate.Month, rtcDate.Date,
            rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds );

    move_cursor(0, 0);
    snprintf(temp, sizeof(temp), "20%02x-%02x-%02x", rtcDate.Year, rtcDate.Month, rtcDate.Date);
    lcd_string(temp);


    move_cursor(1, 0);
    snprintf(temp, sizeof(temp), "%02x:%02x:%02x", rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
    lcd_string(temp);

    HAL_UART_Transmit(&huart2, (uint8_t *)temp2, strlen(temp2), 10);

    HAL_Delay(500);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
