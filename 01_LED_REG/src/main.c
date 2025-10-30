#include "stdint.h"


void delay_count(uint32_t timeCount)      //unsigned int 16bit 임.
{
  for(;timeCount > 0; timeCount--);
}


int main()
{
  // RCC_AHB1 레지스터의 GPIOAEN 인에이블
  (*(volatile unsigned *)0x40023830) |= 0x01;

  // GPIOA 의 모드 설정
  (*(volatile unsigned *)0x40020000) &= ~(0x3 << 10);
  (*(volatile unsigned *)0x40020000) |= (0x1 << 10);

  while(1)
  {
    // 출력
    (*(volatile unsigned *)0x40020014) |= (1 << 5);
    delay_count(500000);
    (*(volatile unsigned *)0x40020014) &= ~(1 << 5);
    delay_count(500000);
  }
}
