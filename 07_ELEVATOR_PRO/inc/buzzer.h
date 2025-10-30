/*
 * buzzer.h
 *
 *  Created on: Oct 20, 2025
 *      Author: user19
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#define BUZZER_Pin        GPIO_PIN_10
#define BUZZER_GPIO_Port  GPIOA


void BuzzerInit(void);

void BuzzerOn(void);

void BuzzerOff(void);

#endif /* INC_BUZZER_H_ */
