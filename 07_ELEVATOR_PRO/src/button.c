#include "button.h"

//  버튼 상태(누름, 안누름) 표시
enum{
  PUSHED,
  RELEASED
};

// 1층, 2층, 3층 버튼 이전 상태
uint8_t prevState1 = RELEASED;
uint8_t prevState2 = RELEASED;
uint8_t prevState3 = RELEASED;

// 1층, 2층, 3층 버튼 현재 상태
uint8_t currState1, currState2, currState3;

// 1층, 2층, 3층 버튼 LED 상태
uint8_t led1, led2, led3;

void buttonState(uint8_t currFloor, uint8_t *targetFloor, uint8_t *move_flag)
{
  // 버튼 누르는 동작 감지
  currState1 = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0) ? PUSHED : RELEASED;
  currState2 = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0) ? PUSHED : RELEASED;
  currState3 = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 0) ? PUSHED : RELEASED;

  // 1층 버튼이 눌리면
  if((prevState1 == RELEASED) && (currState1 == PUSHED))
  {
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    {
      // 1층 버튼 LED ON (or 눌러져있었는데 다시 누르면 OFF)
      if((currFloor == 0b001) && !*move_flag)
      {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);  // 현재 층수가 1층이면 버튼 불 동작x
      }
      else
      {
        led1 = !led1;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, led1); // 현재 층수와 다른 층수 누르면 버튼 불 on/off 가능
        if(led1)
          *targetFloor |= 0b001;            // 1층 예약
        else
          *targetFloor &= ~0b001;           // 1층 예약 도중 해제
      }
    }
  }

  // 2층 버튼이 눌리면
  if((prevState2 == RELEASED) && (currState2 == PUSHED))
  {
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    {
      // 2층 버튼 LED ON (or 눌러져있었는데 다시 누르면 OFF)
      if((currFloor == 0b010) && !*move_flag)
      {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);  // 현재 층 수가 2층이면 버튼 불 동작 x
      }
      else
      {
        led2 = !led2;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, led2); // 현재 층수가 2층이 아니면 버튼 불 on/off 가능
        if(led2)
          *targetFloor |= 0b010;            // 2층 예약
        else
          *targetFloor &= ~0b010;           // 2층 예약 도중 해제
      }
    }
  }

  // 3층 버튼이 눌리면
  if((prevState3 == RELEASED) && (currState3 == PUSHED))
  {
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 0)
    {
      // 3층 버튼 LED ON (or 눌러져있었는데 다시 누르면 OFF)
      if((currFloor == 0b100) && !*move_flag)
      {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);  // 현재 층수가 3층이면 버튼 불 동작 x
      }
      else
      {
        led3 = !led3;

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, led3); // 현재 층수가 3층이 아니면 버튼 불 on/off 가능
        if(led3)
          *targetFloor |= 0b100;            // 3층 예약
        else
          *targetFloor &= ~0b100;           // 3층 예약 도중 해제
      }
    }
  }

  // 현재 상태를 이전 상태로 기억 => 버튼을 누를 때만 LED 동작이 일어나게.
  prevState1 = currState1;
  prevState2 = currState2;
  prevState3 = currState3;
}
