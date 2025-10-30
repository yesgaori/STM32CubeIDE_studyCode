/*
 * stepper.h
 *
 *  Created on: Oct 16, 2025
 *      Author: user14
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "main.h"

extern uint8_t motorState;
typedef enum{
  STOP, UP, DOWN
}State;

extern State elevator;

#define STEPS_PER_REVOLUTION  4096  // 360 ROTATE PER 4096
#define DIR_CW          0   // 시계방향
#define DIR_CCW         1   // 반시계방향

// STEP MOTOR MAGNETIC DRIVE PIN
#define IN1_PIN         GPIO_PIN_1
#define IN1_GPIO_PORT     GPIOB
#define IN2_PIN         GPIO_PIN_15
#define IN2_GPIO_PORT     GPIOB
#define IN3_PIN         GPIO_PIN_14
#define IN3_GPIO_PORT     GPIOB
#define IN4_PIN         GPIO_PIN_13
#define IN4_GPIO_PORT     GPIOB

// STEP MOTOR CYCLE TABLE
static const uint8_t HALF_STEP_SEQ[8][4] =
{
    {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
    {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1},
};

void stepMotor(uint8_t step);

#endif /* INC_STEPPER_H_ */
