
#include "stepper.h"


static const uint8_t HALF_STEP_SEQ[8][4] =
    {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1}
    };


void stepMotor(uint8_t step)
{
  HAL_GPIO_WritePin(IN1_GPIO_PORT, IN1_PIN, HALF_STEP_SEQ[step][0]);
  HAL_GPIO_WritePin(IN2_GPIO_PORT, IN2_PIN, HALF_STEP_SEQ[step][1]);
  HAL_GPIO_WritePin(IN3_GPIO_PORT, IN3_PIN, HALF_STEP_SEQ[step][2]);
  HAL_GPIO_WritePin(IN4_GPIO_PORT, IN4_PIN, HALF_STEP_SEQ[step][3]);
}

void rotateSteps(uint16_t steps, uint8_t direction)
{
  for(uint16_t i = 0; i < steps; i++)
  {
    uint8_t step;
    // 회전방향
    if(direction == DIR_CW) // CW
    {
      step = i % 8;
    }
    else
    {
      step = 7 - (i % 8);   // CCW
    }
    stepMotor(step);

    HAL_Delay(1);           // 각 스텝간 지연
  }
}

void rotateDegrees(uint16_t degrees, uint8_t direction)
{
  // 각도계산

  uint16_t steps = (uint16_t)((uint32_t)(degrees*STEPS_PER_REVOLATION) / 360);

  rotateSteps(steps, direction);
}
