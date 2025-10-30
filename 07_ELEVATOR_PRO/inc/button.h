/*
 * button.h
 *
 *  Created on: Oct 16, 2025
 *      Author: user14
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"              // conf 설정 포함

extern uint8_t led1, led2, led3;

void buttonState(uint8_t currFloor, uint8_t *targetFloor, uint8_t *move_flag);

#endif /* INC_BUTTON_H_ */
