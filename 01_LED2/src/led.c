/*
 * led.c
 *
 *  Created on: Oct 13, 2025
 *      Author: user1
 */


#include "led.h"

LED_CONTROL led[8] =
    {
        {GPIOA, GPIO_PIN_5, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_6, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_7, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOB, GPIO_PIN_6, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOC, GPIO_PIN_7, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_9, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_8, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOB, GPIO_PIN_10, GPIO_PIN_SET, GPIO_PIN_RESET}
    };


void ledOn(uint8_t num)
{
//  // 한꺼번에 num개 켜기
//  for(uint8_t i = 0; i < num; i++)
//  {
//    HAL_GPIO_WritePin(led[i].port, led[i].pinNumber, led[i].onState);
//  }
  uint8_t count = num - 1;
  HAL_GPIO_WritePin(led[count].port, led[count].pinNumber, led[count].onState);
}
void ledOff(uint8_t num)
{
//  // 한꺼번에 num개 끄기
//  for(uint8_t i = 0; i < num; i++)
//  {
//    HAL_GPIO_WritePin(led[i].port, led[i].pinNumber, led[i].offState);
//  }
  uint8_t count = num - 1;
  HAL_GPIO_WritePin(led[count].port, led[count].pinNumber, led[count].offState);
}
void ledToggle(uint8_t num)
{
  HAL_GPIO_TogglePin(led[num].port, led[num].pinNumber);
}

void ledLeftShift(uint8_t num)
{
  for(uint8_t i = 0; i < num; i++)
  {
    ledOn(i + 1);
    HAL_Delay(100);
    ledOff(i + 1);
  }
  HAL_Delay(500);

}
void ledRightShift(uint8_t num)
{
  uint8_t count = num;
  for(uint8_t i = 0 ; i < num; i++)
  {
    ledOn(count);
    HAL_Delay(100);
    ledOff(count);
    count--;
  }
  HAL_Delay(500);
}
void ledFlower(int num)
{

  int petal1 = num/2;
  int petal2 = num/2;

  for(uint8_t i = 1; i <= num/2; i++)
  {
    ledOn(petal1);
    ledOn(petal2 + 1);
    HAL_Delay(150);
    petal1--;
    petal2++;
  }
  HAL_Delay(300);
  for(uint8_t i = 1; i <= num/2; i++)
  {
    petal1++;
    petal2--;
    ledOff(petal1);
    ledOff(petal2 + 1);
    HAL_Delay(150);

  }
  HAL_Delay(300);

//  for(uint8_t i = 0; i < num/2; i++)
//  {
//    ledToggle(petal1);
//    ledToggle(petal2 - 1);
//    HAL_Delay(200);
//    petal1 ++;
//    petal2 --;
//  }

}

