#include "main.h"

/*
Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
R:测量噪声，R增大，动态响应变慢，收敛稳定性变好
x_last=x(k-1|k-1)上一时刻的距离    p_last=p(k-1|k-1)上一时刻的协方差，上个时刻的迭代值
x_mid=x(k|k-1)，这一时刻的预测距离    p_mid=p(k|k-1)预测协方差
z_measure 测量值    R测量噪声，可理解为测量协方差
x_now计算得出的本时刻距离，p_now，计算出的本时刻协方差，供下一时刻迭代
*/
void kalmanfilter(int16_t *z_measure)
{
  static float x_mid,x_last=0,p_mid,p_last=0,p_now,x_now=0;
  static float Q=ProcessNiose_Q,kg,R=MeasureNoise_R;
  static int8_t init_flag=0;
  if(!init_flag)
  {
    init_flag=1;
    x_last=*z_measure;
  }
  x_mid=x_last;                       //预测与上一时刻一致
  p_mid=p_last+Q;                     //预测协方差为上一时刻协方差+Q_Noise
  kg=p_mid/(p_mid+R);                 //kg为kalman filter，R 为测量噪声
  x_now=x_mid+kg*(*z_measure-x_mid);  //计算出的最优值
  p_now=(1-kg)*p_mid;                   //最优值对应的covariance
  p_last=p_now;                         //更新covariance 值
  x_last=x_now;                            //更新系统状态值
  *z_measure=(int)x_now;
}



void limitfilter(int16_t *measure)
{
  if(*measure>UPPER_LIMIT)
  {
    *measure=UPPER_LIMIT;
  }
}
