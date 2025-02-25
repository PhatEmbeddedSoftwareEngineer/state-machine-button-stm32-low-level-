/*
 * systick.c
 *
 *  Created on: Jan 27, 2025
 *      Author: Phat
 */

 #include "systick.h"

 void delay_ms(int ms)
 {
     /**
      * muốn tạo delay 1ms áp dụng công thức do đang chọn oscilicator clock là xung nhịp nội nên ta có hsi = 16mhz mới tính được ở dưới
      * N = delay_achieved * SYSCLK (defalut SYSCLK == frequency default hsi = 16 000 000)
      * N = 1ms * 16 000 000 = 16 000
      * N will value reload for SYStick reload register
      * so SYSTICK->SYST_RVR = 16000
      */
     SYSTICK->SYST_RVR = 16000; // 16 000 000 / 16 000
     SYSTICK->SYST_CVR = 0; // value current =0
     //SYSTICK->SYST_CSR |= (1U << 0) | (1U << 2); //
     SYSTICK->SYST_CSR |= INTERNAL_CLOCK | COUNTER_EN;
 
 
     for(int i=0; i <ms;i++)
     {
         while(!(SYSTICK->SYST_CSR & COUNTFLAG));
     }
     SYSTICK->SYST_CSR = 0;
 }
 
 void create_1hz_interrupt_systick()
 {
     // giá trị nạp vào thanh ghi reload 
     SYSTICK->SYST_RVR = 16000000-1;
     // giá trị hiện tại
     SYSTICK->SYST_CVR = 0; 
     // cấu hình dùng thạch anh nội bắt đầu đếm và dùng ngắt
     SYSTICK->SYST_CSR |= INTERNAL_CLOCK | COUNTER_EN | INTERRUPT_SYSTICK_EN;
 
 }
//  static void callBackSystick(void)
//  {
//      static float a=0.0f;
//      //myPrintf("hello world: %.2f\n",a++);
//  }
 
//  void SysTick_Handler(void)
//  {
//      callBackSystick();
//  }
 
 
 