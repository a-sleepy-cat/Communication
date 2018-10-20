#include "main.h"

u8 judge_result;        //全局变量用于存放判断火源的结果。
void judge_fire()       //判断是否有火源
{
  u16 tem;
  float temp1,temp3;
  tem=Get_Adc_Average(ADC_Channel_10,1);
  temp1=(float)tem*3.3/4096;
  
  tem=Get_Adc_Average(ADC_Channel_12,1);
  temp3=(float)tem*3.3/4096;

  if(temp1<1){
    judge_result=fire_on_left;
    FSMFlagNext=PUTOUTFLAG;
  }
  
  else if(temp3<1){
    judge_result=fire_on_right;
    FSMFlagNext=PUTOUTFLAG;
  }
  else
    judge_result=no_fire;
}
