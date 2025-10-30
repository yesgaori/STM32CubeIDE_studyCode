

#include "button.h"

BUTTON_CONTROL button[3]=
    {
        {GPIOB, GPIO_PIN_3, 0},
        {GPIOB, GPIO_PIN_5, 0},
        {GPIOB, GPIO_PIN_4, 0}
    };
/*
 * Blocking code
 *
 */

//bool buttonGetPressed(uint8_t num)
//{
//  bool ret = false;
//
//  if(HAL_GPIO_ReadPin(button[num].port, button[num].pinNumber) == button[num].onState)
//  {
//    HAL_Delay(20);
//    if(HAL_GPIO_ReadPin(button[num].port, button[num].pinNumber) == button[num].onState)
//    {
//      ret = true;
//    }
//  }
//
//}

/*
 * Non-Blocking code
 */

//bool buttonGetPressed(uint8_t num)
//{
//  static uint32_t prevTime = 0;   // 최초 첫번째 디바운스가 안됨
//
//  bool ret = false;
//
//  if(HAL_GPIO_ReadPin(button[num].port, button[num].pinNumber) == button[num].onState)
//  {
//    uint32_t currTime = HAL_GetTick();
//    if(currTime - prevTime > 50)
//    {
//      if(HAL_GPIO_ReadPin(button[num].port, button[num].pinNumber) == button[num].onState)
//          {
//            ret = true;
//          }
//      prevTime = currTime;
//    }
//  return ret;
//  }
//
//}

/*
 * Non-Blocking -2
 */

//bool buttonGetPressed(uint8_t num)
//{
//  static uint32_t prevTime = 0xFFFFFFFF;
//
//  if(prevTime == 0xFFFFFFFF)
//  {
//    prevTime = HAL_GetTick();
//  }
//
//  bool ret = false;
//
//  if(HAL_GPIO_ReadPin(button[num].port, button[num].pinNumber) == button[num].onState)
//  {
//    uint32_t currTime = HAL_GetTick();
//    if(currTime - prevTime > 50)
//    {
//      if(HAL_GPIO_ReadPin(button[num].port, button[num].pinNumber) == button[num].onState)
//          {
//            ret = true;
//          }
//      prevTime = currTime;
//    }
//
//  }
//  return ret;
//}

/*
 *Non_Blocking -3
 */


bool buttonGetPressed(uint8_t num)
{
  static uint32_t prevTime[3] = {0xFFFFFFFF};

  if(prevTime[num] == 0xFFFFFFFF)
  {
    prevTime[num] = HAL_GetTick();
  }

  bool ret = false;

  if(HAL_GPIO_ReadPin(button[num].port, button[num].pinNumber) == button[num].onState)
  {
    uint32_t currTime = HAL_GetTick();
    if(currTime - prevTime[num] > 150)
    {
      ret = true;
      prevTime[num] = currTime;
    }

  }
  return ret;
}
