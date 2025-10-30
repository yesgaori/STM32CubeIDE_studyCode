/*
 * led.c
 *
 *  Created on: Oct 13, 2025
 *      Author: user1
 */


#include "led.h"


void updateLed(LED_CONTROL *led)
{
  uint32_t currentTick = HAL_GetTick();   // 현재의 틱(카운트)를 받아서 저장

  if(currentTick - led -> lastTick >= led -> interval)
  {
    led -> lastTick = currentTick;

    if(led -> state == LED_OFF)
    {
      HAL_GPIO_WritePin(led -> port, led -> pinNumber, 1);
      led -> state = LED_ON;
    }
    else
    {
      HAL_GPIO_WritePin(led -> port, led -> pinNumber, 0);
      led -> state = LED_OFF;
    }
  }
}

