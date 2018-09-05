#include "main.h"

static u8  fac_us=0;              //us延时倍乘数

/**
  * @brief 内联汇编代码，掩蔽所有中断
  * @param None
  * @retval None
  * @details 掩蔽期间中断被悬期，结束掩蔽后这些中断将被执行，若不想执行，请清除中断标志位
  */
void NVIC_SETPRIMASK(void)
{
  __asm__ volatile("CPSID i");
}

/**
  * @brief 内联汇编代码，结束掩蔽所有中断
  * @param None
  * @retval None
  * @details 掩蔽期间中断被悬期，结束掩蔽后这些中断将被执行，若不想执行，请清除中断标志位
  */
void NVIC_RESETPRIMASK(void)
{
  __asm__ volatile("CPSIE i");		  
}

void SysTick_Init()
{
  u32 reload=0;
  fac_us=SystemCoreClock/8000000;                          //每个us需要的systick时钟数
  reload=SystemCoreClock/8000*5;                           //reload值为5ms需要的systick时钟数
  SysTick->LOAD  = (reload & SysTick_LOAD_RELOAD_Msk) - 1; //每5ms中断一次
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
  SysTick->VAL   = 0;
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);    //选择外部时钟  HCLK/8
  SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;                 //开始SYSTICK中断
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;                  //开始SYSTICK计数

#if DRIVER_CHECK
printf("SysTick init successful\r\n");
#endif
}

void delay_us(u32 nus)
{
	NVIC_SETPRIMASK();
  u32 tick=nus*fac_us;
  u32 time_old,time_now,time_cnt=0;
  u32 reload=SysTick->LOAD;

  time_old=SysTick->VAL;
  while(1)
  {
    time_now=SysTick->VAL;
    if(time_now<=time_old)
      time_cnt+=time_old-time_now;
    else
      time_cnt+=reload-time_now+time_old;

    time_old=time_now;
    if(time_cnt>=tick)
      break;
  }
	NVIC_RESETPRIMASK();
}

void delay_ms(u16 nms)
{
  delay_us((u32)(1000*nms));
} 


//注释掉了位于stm32f10x_it.c 136行的SysTick_Handler(),避免重定义。
void SysTick_Handler(void)
{

#if IRQONCE_CHECK
  static u8 systickIRQ_flag=1;
  if(systickIRQ_flag)
  {
    printf("SysTick_Handler execute successful!\r\n");
    systickIRQ_flag=0;
  }
#elif IRQDUPLICATE_CHECK
  printf("SysTick_Handler execute successful!\r\n");
#endif

  taskloop();
}
