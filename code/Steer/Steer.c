/*
 * Steer.c
 *
 *  Created on: 2025年10月25日
 *      Author: LDLS
 */
#include "Steer.h"

int16 steer_debug_pwm = Steer_MID;

void Steer_Init(void)  //舵机初始化
{
    pwm_init(ATOM3_CH3_P11_6,300,Steer_MID);
}

void Steer_Control(uint32 duty)  //舵机控制
{
    if (duty > Steer_left){duty = Steer_left;}
    if (duty < Steer_right){duty = Steer_right;}
    pwm_set_duty(ATOM3_CH3_P11_6, duty);
}

void Steer_PD(void)  //舵机pd控制
{
    if (Steer_status == status_steer_start)
    {
          ImageStatus.Steer_Center_error = Get_Steer_Error();
          float steer_output = Positional_PID(&Steer_PID, 0, ImageStatus.Steer_Center_error, 10000);
          int PWM = Steer_MID + (int)steer_output;
          Steer_Control(PWM);
    }
    else if (Steer_status == status_steer_stop)
    {
          ImageStatus.Steer_Center_error = Get_Steer_Error();
          Steer_Control(Steer_MID);
    }
}



