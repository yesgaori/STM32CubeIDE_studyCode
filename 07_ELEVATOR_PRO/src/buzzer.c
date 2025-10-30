/*
 * buzzer.c
 *
 *  Created on: Oct 20, 2025
 *      Author: user19
 */

#include "main.h"
#include "buzzer.h"


void BuzzerInit(void)       // 부저 초기상태
{
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET );
}

void BuzzerOn(void)       // 부저 켜기
{
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
}

void BuzzerOff(void)        // 부저 끄기
{
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);
}
