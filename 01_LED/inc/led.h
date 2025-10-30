/*
 * led.h
 *
 *  Created on: Oct 13, 2025
 *      Author: user1
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"

typedef enum
{
 LED_OFF,
 LED_ON
}LED_STATE;

typedef struct
{
  GPIO_TypeDef  *port;
  uint16_t      pinNumber;
  uint32_t      interval;   // delay time
  LED_STATE     state;
  uint32_t      lastTick;   // time
}LED_CONTROL;

void updateLed(LED_CONTROL *led);

#endif /* INC_LED_H_ */
