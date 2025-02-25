#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "systick.h"
#include <stdbool.h>

#define DEBOUND_TIME  50
#define WAIT_3S       800

typedef enum
{
  NO_PRESS,
  PRESS,
  LONG_PRESS,
}Button_state;

typedef struct 
{
  bool isPress;
  uint16_t timePress;
  Button_state state;
}Button_t;
bool buttonStatePC13();
Button_state read_stateButton(Button_t *button)
{
  button->state = NO_PRESS;
  while(!buttonStatePC13())
  {
    button->isPress = true;
    button->timePress++;
    delay_ms(1);
  }
  if(button->isPress)
  {
    while(buttonStatePC13())
    {
      if(button->timePress >= DEBOUND_TIME && button->timePress <= WAIT_3S)
      {
        button->timePress = 0;
        button->isPress = false;
        button->state = PRESS;
        return button->state;
      }
      else if(button->timePress > WAIT_3S)
      {
        button->timePress = 0;
        button->isPress = false;
        button->state = LONG_PRESS;
        return button->state;
      }
    }
  }
  return button->state;
}


void init_tx_uart(void)
{
    volatile uint32_t *RCC_AHB1ENR = (uint32_t*)(0x40023800 + 0x30);
    *RCC_AHB1ENR |= (1U << 0);

    volatile uint32_t *GPIOA_MODER = (uint32_t*)(0x40020000 + 0x00);
    *GPIOA_MODER &= ~ (0b11 << 18);
    *GPIOA_MODER |= (0b10 << 18);

    volatile uint32_t *GPIOA_AFRH = (uint32_t*)(0x40020000 + 0x24);
    *GPIOA_AFRH &= ~ (0b1111 << 4);
    *GPIOA_AFRH |= (0b0111 << 4);

    volatile uint32_t *RCC_APB2ENR = (uint32_t*)(0x40023800 + 0x44);
    *RCC_APB2ENR &= ~ (1U << 4);
    *RCC_APB2ENR |= (1U << 4);

    volatile uint32_t *USART1_CR1 = (uint32_t*)(0x40011000 + 0x0C);
    *USART1_CR1 &= ~ (1U << 15);

    volatile uint32_t *USART1_BRR = (uint32_t*)(0x40011000 + 0x08);
    *USART1_BRR |= (8U << 4) | (11U << 0);

    *USART1_CR1 |= (1U << 3);
    *USART1_CR1 |= (1U << 13);
}

void transmissonEachByte(char *buf)
{
  volatile uint32_t *USART1_DR = (uint32_t*)(0x40011000 + 0x04);
  volatile uint32_t *USART1_SR = (uint32_t*)(0x40011000 + 0x00);
  while(*buf)
  {
    *USART1_DR = *buf++;
    while(!(*USART1_SR & (1U << 7)));
  }
}

void myPrintf(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);
    char buffer[256];
    vsnprintf(buffer,sizeof(buffer),fmt,arg);
    transmissonEachByte(buffer);
    va_end (arg);
}

void initButton()
{
  volatile uint32_t *RCC_AHB1ENR = (uint32_t*)(0x40023800 + 0x30);
  *RCC_AHB1ENR |= (1U << 2);
  // pc13
  volatile uint32_t *GPIOC_MODER = (uint32_t*)(0x40020800 + 0x00);
  *GPIOC_MODER &= ~ (3U << 26);

}

bool buttonStatePC13()
{
  volatile uint32_t *GPIOC_IDR = (uint32_t *)(0x40020800 + 0x10);
  return (*GPIOC_IDR & (1U << 13));
}


int main(void)
{
  init_tx_uart();
  initButton();
  //int cnt = 0;
  Button_state currentStateButton;
  Button_t obj ={
    .isPress = false,
    .state = NO_PRESS,
    .timePress = 0,
  };
  while(1)
  {
    currentStateButton = read_stateButton(&obj);
    if(currentStateButton != 0)
    {
      if(currentStateButton == LONG_PRESS)
      {
        myPrintf("LONG PRESS\n");
      }
      else if(currentStateButton == PRESS)
      {
        myPrintf("PRESS\n");
      }
      //myPrintf("currentStateButton: %d\n",currentStateButton);
    }
  }
}
