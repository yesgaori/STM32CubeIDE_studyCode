/*
 * fnd.c
 *
 *  Created on: Oct 17, 2025
 *      Author: user19
 */

#include "fnd.h"


/* Common Cathode  켜기=GPIO_PIN_SET, 끄기=GPIO_PIN_RESET */
static void segWrite(GPIO_TypeDef* port, uint16_t pin, uint8_t on)
{
  HAL_GPIO_WritePin(port, pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* 0~9 패턴 (bit6..0 = a b c d e f g, 1=ON) */
static const uint8_t s_digit[10] =
{
  0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011  // 9
};

/**
 * FND_Init
 * 전원넣기
 */
void FND_Init(void)
{
  FND_Clear();  // 시작 상태: 모두 끔
}

/**
 * FND_Clear
 * 클리어
 */
void FND_Clear(void)
{
  segWrite(FND_A_PORT, FND_A_PIN, 0);
  segWrite(FND_B_PORT, FND_B_PIN, 0);
  segWrite(FND_C_PORT, FND_C_PIN, 0);
  segWrite(FND_D_PORT, FND_D_PIN, 0);
  segWrite(FND_E_PORT, FND_E_PIN, 0);
  segWrite(FND_F_PORT, FND_F_PIN, 0);
  segWrite(FND_G_PORT, FND_G_PIN, 0);
}

/**
 * FND_WriteDigit
 * 숫자 하나 출력하기
 */
void FND_WriteDigit(uint8_t digit)
{
  if (digit < 10)
    FND_WriteSegments(s_digit[digit]);
  else
    FND_Clear();
}

/**
 * FND_WriteSegments
 * 세그먼트를 직접 켜거나 끄기
 */
void FND_WriteSegments(uint8_t mask)
{
  segWrite(FND_A_PORT, FND_A_PIN, (mask>>6)&1);
  segWrite(FND_B_PORT, FND_B_PIN, (mask>>5)&1);
  segWrite(FND_C_PORT, FND_C_PIN, (mask>>4)&1);
  segWrite(FND_D_PORT, FND_D_PIN, (mask>>3)&1);
  segWrite(FND_E_PORT, FND_E_PIN, (mask>>2)&1);
  segWrite(FND_F_PORT, FND_F_PIN, (mask>>1)&1);
  segWrite(FND_G_PORT, FND_G_PIN, (mask>>0)&1);
}




