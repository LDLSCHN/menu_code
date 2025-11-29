/*
 * Motor.c
 *
 *  Created on: 2025年10月24日
 *      Author: LDLS
 */
#include "Motor.h"
#include "PID.h"
#include "MyEncoder.h"

//左轮
int16 Speed_Goal_l;//目标速度
int16 Speed_Encoder_l;//当前速度
float Speed_P_l,Speed_I_l,Speed_D_l;
int16 Speed_Erro_l;//当前误差
int16 Speed_PID_OUT_l;//占空比输出
int16 Speed_Lasterro_l;//上次误差
int16 Speed_Preverro_l;//上上次误差

//右轮
int16 Speed_Goal_r;//目标速度
int16 Speed_Encoder_r;//当前速度
float Speed_P_r,Speed_I_r,Speed_D_r;
int16 Speed_Erro_r;//当前误差
int16 Speed_PID_OUT_r;//占空比输出
int16 Speed_Lasterro_r;//上次误差
int16 Speed_Preverro_r;//上上次误差

int16 Speed_Goal;
int16 L_ring_speed_fast = 100;
int16 L_ring_speed_slow = 80;
int16 R_ring_speed_fast = 130;
int16 R_ring_speed_slow = 110;
int16 cross_speed = 90;
int16 set_speed = 120;
int16 speed1, speed2;
PID_IncTypeDef PID;
//Car_Statustypedef Carstatus;

//dir给1反转

void Motor_Init(void)     //电机初始化
{
//    pwm_init(ATOM3_CH4_P11_9,10000,2000);   //左电机pwm
//    gpio_init(P11_10, GPO, 0, GPO_PUSH_PULL);     //左电机dir
//    pwm_init(ATOM3_CH6_P11_11,10000,2000);  //右电机pwm
//    gpio_init(P11_12, GPO, 0, GPO_PUSH_PULL);     //右电机dir
      pwm_init(ATOM3_CH7_P11_12,10000, 0);    //左电机pwm
      gpio_init(P11_11, GPO, 0, GPO_PUSH_PULL);   //左电机dir
      pwm_init(ATOM3_CH5_P11_10,10000, 0);   //右电机pwm
      gpio_init(P11_9, GPO, 0, GPO_PUSH_PULL);   //右电机dir
}

void Speed_PIDL(void) //左轮速度环
{
    Speed_Encoder_l = speed1;
//    Speed_PID_OUT_l = (int16) Changing_Integration_Rate_Positional_PID(&Motor_Speed_PID_l,(float)Speed_Goal_l,(float)Speed_Encoder_l,Motor_Speed_PID_l.I_Max, 30, 15);
    Speed_PID_OUT_l = (int16) Positional_PID(&Motor_Speed_PID_l,(float)Speed_Goal_l,(float)Speed_Encoder_l,6000);
    if(Speed_PID_OUT_l>=0)
    {
        if(Speed_PID_OUT_l > 8000) Speed_PID_OUT_l = 8000;
        pwm_set_duty(ATOM3_CH7_P11_12,Speed_PID_OUT_l);
        gpio_set_level(P11_11,0);
    }
    else
    {
        if(Speed_PID_OUT_l < -8000) Speed_PID_OUT_l = -8000;
        pwm_set_duty(ATOM3_CH7_P11_12,-Speed_PID_OUT_l);
        gpio_set_level(P11_11,1);
    }
}

void Speed_PIDR(void) //右轮速度环
{
    Speed_Encoder_r = speed2;
//    Speed_PID_OUT_r = (int16) Changing_Integration_Rate_Positional_PID(&Motor_Speed_PID_r,(float)Speed_Goal_r,(float)Speed_Encoder_r,Motor_Speed_PID_r.I_Max, 30, 15);
    Speed_PID_OUT_r = (int16) Positional_PID(&Motor_Speed_PID_r,(float)Speed_Goal_r,(float)Speed_Encoder_r,6000);
    if(Speed_PID_OUT_r>=0)
    {
        if(Speed_PID_OUT_r > 8000) Speed_PID_OUT_r = 8000;
        pwm_set_duty(ATOM3_CH5_P11_10,Speed_PID_OUT_r);
        gpio_set_level(P11_9,0);
    }
    else
    {
        if(Speed_PID_OUT_r < -8000) Speed_PID_OUT_r = -8000;
        pwm_set_duty(ATOM3_CH5_P11_10,-Speed_PID_OUT_r);
        gpio_set_level(P11_9,1);
    }
}

void Control_Speed(void)
{
//    Speed_Goal = set_speed;
//    // ==================== 左圆环速度决策 ====================
//    if (L_round_status == status_L_round_on)
//    {
//        if (L_roundabout_state == 0)
//        {
//            Speed_Goal = set_speed;  // 正常巡线
//        }
//        else if (L_roundabout_state == 1)
//        {
//            Speed_Goal = L_ring_speed_fast;  // 入环降速
//        }
//        else if (L_roundabout_state == 2)
//        {
//            Speed_Goal = L_ring_speed_slow;  // 环内最慢
//        }
//        else if (L_roundabout_state == 3)
//        {
//            Speed_Goal = L_ring_speed_fast;  // 出环稍快
//        }
//    }
//    // ==================== 右圆环速度决策 ====================
//    else if (R_round_status == status_R_round_on)
//    {
//        if (R_roundabout_state == 0)
//        {
//            Speed_Goal = set_speed;
//        }
//        else if (R_roundabout_state == 1)
//        {
//            Speed_Goal = R_ring_speed_slow;
//        }
//        else if (R_roundabout_state == 2)
//        {
//            Speed_Goal = R_ring_speed_slow;
//        }
//        else if (R_roundabout_state == 3)
//        {
//            Speed_Goal = R_ring_speed_fast;
//        }
//    }
//    if (cross_status == status_cross_on)
//    {
//        if (cross_state == 0)
//        {
//            Speed_Goal = set_speed;
//        }
//        if (cross_state == 1)
//        {
//            Speed_Goal = cross_speed;
//        }
//    }
//    if (is_long_straight)
//    {
//        Speed_Goal = 110;  // 加速20%
//    }
    Speed_Goal = set_speed;
    if (cross_status == status_cross_on && cross_state != 0)
    {
        Speed_Goal = 110;
        return;
    }

    // ==================== 优先级2: 左圆环 ====================
    if (L_round_status == status_L_round_on && L_roundabout_state != 0)
    {
        switch (L_roundabout_state)
        {
            case 1:  // 入环/摆正阶段
                Speed_Goal = L_ring_speed_fast;
                break;
            case 2:  // 环内阶段
                Speed_Goal = L_ring_speed_slow;
                break;
            case 3:  // 出环阶段
                Speed_Goal = L_ring_speed_fast;
                break;
            default:
                Speed_Goal = set_speed;
                break;
        }
        return;  // ✅ 直接返回
    }

    // ==================== 优先级3: 右圆环 ====================
    if (R_round_status == status_R_round_on && R_roundabout_state != 0)
    {
        switch (R_roundabout_state)
        {
            case 1:  // 入环阶段
                Speed_Goal = set_speed;
                break;
            case 2:  // 环内阶段
                Speed_Goal = R_ring_speed_slow;
                break;
            case 3:  // 出环阶段
                Speed_Goal = R_ring_speed_fast;
                break;
            default:
                Speed_Goal = set_speed;
                break;
        }
        return;
    }
    if (is_long_straight)
    {
        Speed_Goal = 140;
        return;
    }
    float abs_error = fabsf(Get_Steer_Error());

    if (abs_error > 42.0f)
    {
        Speed_Goal = 100;
        return;
    }
}

void Get_Speed_L(void)  //获取左轮速度
{
    speed1 = Encoder_MTM(TIM2_ENCODER,1,1);
}

void Get_Speed_R(void)  //获取右轮速度
{
    speed2 = Encoder_MTM(TIM6_ENCODER,1,1);
}

void Motor_Turn(void)   //差速转向环PD
{
    float error = Get_Motor_Error() / 10;
    int PID_out = Positional_PID(&Turn_PID, 0, error, 3000);
//    printf("%d\r\n",PID_out);

    Speed_Goal_l = Speed_Goal - PID_out;
    Speed_Goal_r = Speed_Goal + PID_out;
}

void Motor_stop(void)
{
    pwm_set_duty(ATOM3_CH7_P11_12,0);
    pwm_set_duty(ATOM3_CH5_P11_10,0);
}

void Speed_Measure(void)
{
    Get_Speed_L();
    Get_Speed_R();
    if (g_Car_Status == status_car_start)
    {
         Control_Speed();
         Motor_Turn();
         Speed_PIDL();
         Speed_PIDR();
    }
    else if (g_Car_Status == status_car_stop)
    {
        Motor_stop();
        Speed_Encoder_l = speed1;
        Speed_Encoder_r = speed2;
    }
}

