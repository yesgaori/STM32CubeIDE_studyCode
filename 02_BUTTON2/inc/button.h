/*
 * button.h
 *
 *  Created on: Oct 14, 2025
 *      Author: user1
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#include "stdbool.h"

typedef struct
{
  GPIO_TypeDef  *port;
  uint16_t      pinNumber;
  GPIO_PinState onState;
}BUTTON_CONTROL;

bool buttonGetPressed(uint8_t num);


#endif /* INC_BUTTON_H_ */
