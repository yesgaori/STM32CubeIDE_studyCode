/*
 * fnd.h
 *
 *  Created on: Oct 17, 2025
 *      Author: user19
 */

#ifndef INC_FND_H_
#define INC_FND_H_

#include "main.h"

#define FND_COMMON_ANODE  0   // Common cathode를 사용하기에 기본값 0

#define FND_A_PORT  GPIOA
#define FND_A_PIN   GPIO_PIN_6
#define FND_B_PORT  GPIOA
#define FND_B_PIN   GPIO_PIN_7
#define FND_C_PORT  GPIOB
#define FND_C_PIN   GPIO_PIN_6
#define FND_D_PORT  GPIOC
#define FND_D_PIN   GPIO_PIN_7
#define FND_E_PORT  GPIOA
#define FND_E_PIN   GPIO_PIN_9
#define FND_F_PORT  GPIOA
#define FND_F_PIN   GPIO_PIN_8
#define FND_G_PORT  GPIOB
#define FND_G_PIN   GPIO_PIN_10

void FND_Init(void);
void FND_Clear(void);
void FND_WriteDigit(uint8_t digit);
void FND_WriteSegments(uint8_t mask);


#endif /* INC_FND_H_ */

