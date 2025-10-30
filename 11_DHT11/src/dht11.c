#include "dht11.h"


// DHT11 초기화
void dht11Init(DHT11 *dht, GPIO_TypeDef *port, uint16_t pin)
{
  // 구조체의 포트와 핀번호 설정
  dht -> port = port;
  dht -> pin = pin;
}

void dht11GpioMode(DHT11 *dht, uint8_t mode)
{
  // GPIOC 핀에 대한 구조체 선언 및 초기화
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // 출력 모드 설정
  if(mode == OUTPUT)            // 출력 모드 설정
  {
    GPIO_InitStruct.Pin = dht -> pin;                 // 핀번호를 우리가 지정한 구조체 멤버(핀 번호)
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(dht -> port, &GPIO_InitStruct);     // 포트를 우리가 지정한 구조체 멤버(포트)
  }
  else if(mode == INPUT)        // 입력 모드 설정
  {
    GPIO_InitStruct.Pin = dht -> pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(dht -> port, &GPIO_InitStruct);
  }

}

// DHT11 데이터 읽기
uint8_t dht11Read(DHT11 *dht)
{
  bool ret = true;

  uint16_t timeTick = 0;
  uint8_t pulse[40] = {0};      // 40비트의 데이터를 저장할 배열

  // 온습도 변수
  uint8_t humValue1 = 0, humValue2 = 0;
  uint8_t tempValue1 = 0, tempValue2 = 0;
  uint8_t parityValue = 0;            // 체크썸

  HAL_TIM_Base_Start(&htim11);

  // 통신신호를 전송
  dht11GpioMode(dht, OUTPUT);   // 출력설정
  HAL_GPIO_WritePin(dht -> port, dht -> pin, 0);
  HAL_Delay(20);
  HAL_GPIO_WritePin(dht -> port, dht -> pin, 1);
  delay_us(30);
  dht11GpioMode(dht, INPUT);    // 입력설정

  // DHT11 응답 신호 대기
  __HAL_TIM_SET_COUNTER(&htim11, 0);    // 카운터 초기화
  while(HAL_GPIO_ReadPin(dht -> port, dht -> pin) == GPIO_PIN_RESET)     // LOW 신호대기
  {
    if(__HAL_TIM_GET_COUNTER(&htim11) > 100)
    {
      printf("Low Signal Time Out\r\n");
      return 0;
    }
  }

  __HAL_TIM_SET_COUNTER(&htim11, 0);  // 카운터 초기화
  while(HAL_GPIO_ReadPin(dht -> port, dht -> pin) == GPIO_PIN_SET)     // HIGH 신호대기
  {
    if(__HAL_TIM_GET_COUNTER(&htim11) > 100)
    {
      printf("High Signal Time Out\r\n");
      return 0;
    }

  }

  // 데이터 수신
  for(uint8_t i = 0; i < 40; i++)   // 40bit 루프
  {
    while(HAL_GPIO_ReadPin(dht -> port, dht -> pin) == GPIO_PIN_RESET); // 50us 동안 대기

    __HAL_TIM_SET_COUNTER(&htim11, 0);
    while(HAL_GPIO_ReadPin(dht -> port, dht -> pin) == GPIO_PIN_SET)
    {
      timeTick = __HAL_TIM_GET_COUNTER(&htim11);    // High signal 카운터 측정

      // 신호 길이를 구분 0, 1
      if(timeTick > 20 && timeTick < 30)
      {
        pulse[i] = 0;
      }
      else if(timeTick > 65 && timeTick < 85)
      {
        pulse[i] = 1;
      }
    }
  }
  HAL_TIM_Base_Stop(&htim11);

  // 온습도 데이터 처리
  for(uint8_t i = 0; i < 8; i++)
  {
    humValue1 = (humValue1 << 1) + pulse[i];    // 습도 상위 8비트
  }
  for(uint8_t i = 8; i < 16; i++){ humValue2 = (humValue2 << 1) + pulse[i]; }
  for(uint8_t i = 16; i < 24; i++){ tempValue1 = (tempValue1 << 1) + pulse[i]; }
  for(uint8_t i = 24; i < 32; i++){ tempValue2 = (tempValue2 << 1) + pulse[i]; }

  for(uint8_t i = 32; i < 40; i++){ parityValue = (parityValue << 1) + pulse[i]; }    // check Sum

  // 우리의 구조체 멤버에 값을 입력
  dht -> temperature = tempValue1;    // 온도 데이터저장
  dht -> humidity = humValue1;         // 습도 데이터저장

  // 데이터 무결성 검증
  uint8_t checkSum = humValue1 + humValue2 + tempValue1 + tempValue2;
  if(checkSum != parityValue)
  {
    printf("check sum Error \n\r");
    ret = false;
  }

  return ret;
}
