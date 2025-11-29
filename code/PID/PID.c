/*
 * pid.c
 *
 * Created on: 2024年1月24日
 *     Author: 潘申奇
 */
#include"PID.h"
#define MY_ABS(num) (((num) > 0) ? (num) : -(num))
#define LIMIT_VAL(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))

PID_IncTypeDef Motor_Speed_PID_l;
PID_IncTypeDef Motor_Speed_PID_r;
PID_IncTypeDef Steer_PID;
PID_IncTypeDef Turn_PID;

void PID_param_init(void)
{
    PID_Inc_Init(&Motor_Speed_PID_l, 21, 0.95, 0, 6000);
    PID_Inc_Init(&Motor_Speed_PID_r, 21, 0.95, 0, 6000);
    PID_Inc_Init(&Steer_PID, 9, 0, 0, 5000);
    PID_Inc_Init(&Turn_PID,3,0,0,3000);
}

/*
函数名称：PID_Inc_Init(PID_IncTypeDef *sptr, float kp, float ki, float kd)
函数功能：初始化PID参数
函数变量：*sptr：pid参数结构体
例子：
*/
void PID_Inc_Init(PID_IncTypeDef *sptr, float kp, float ki, float kd, float i_max)
{
    sptr->last_Ek = 0; // 上次偏差值初始化
    sptr->Ek_sum = 0; // 上上次偏差值初始化
    sptr->Kp = kp; // 比例常数
    sptr->Ki = ki; // 积分常数
    sptr->Kd = kd; // 微分常数
    sptr->OUT = 0;
    sptr->OUT1 = 0;
    sptr->P_Out = 0;
    sptr->I_Out = 0;
    sptr->D_Out = 0;
    sptr->OLS_Order = 0;
    sptr->I_Max = i_max;

}

/*
函数名称：Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I)
函数功能：位置式pid
函数变量：*sptr：pid参数结构体
          SetValue：设定值
          ActualValue：实际值
          Max_I：最大误差积分
例子：
*/
float Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I)
{
    float PIDInc;
    PID->Ek = SetValue - ActualValue;
    PID->Ek_sum += PID->Ek;

    if (PID->Ek_sum > Max_I)
        PID->Ek_sum = Max_I;
    if (PID->Ek_sum < -Max_I)
        PID->Ek_sum = -Max_I;

    PIDInc = (PID->Kp * PID->Ek) +
             (PID->Ki * PID->Ek_sum) +
             (PID->Kd * (PID->Ek - PID->last_Ek));
    PID->last_Ek = PID->Ek;
    return PIDInc;
}

/*
函数名称：Incremental_PID
函数功能：增量式PID
函数变量：
    *PID: PID参数结构体
    SetValue: 设定值
    ActualValue: 实际值
例子：
*/
/***********************************************
* @brief : 增量式PID
* @param : *PID: PID参数结构体
            SetValue：设定值
            ActualValue：实际值
* @return: 返回值
* @date  : 修改日期
* @author: 作者
************************************************/
float Incremental_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue)
{


    // 计算当前偏差
    PID->Ek = SetValue - ActualValue;

    // 计算PID增量公式
    PID->OUT = ((PID->Kp * (PID->Ek - PID->last_Ek)) +                   // P部分：当前偏差与上次偏差之差
                 (PID->Ki * PID->Ek)) +                                    // I部分：当前偏差积分
                 (PID->Kd * (PID->Ek - 2 * PID->last_Ek + PID->Ek_sum));  // D部分：当前偏差、上次偏差与上上次偏差之组合

    // 更新偏差值
    PID->Ek_sum = PID->last_Ek;  // 保存上上次偏差
    PID->last_Ek = PID->Ek;   // 保存上次偏差

    if(PID->OUT >= 6000)
        PID->OUT = 6000;

    // 返回增量值
    return PID->OUT;
}


/*
函数名称：PID_clear(PID_IncTypeDef *sptr)
函数功能：清空误差
函数变量：*sptr：pid参数结构体
例子：
*/
void PID_clear(PID_IncTypeDef *sptr)
{
    sptr->last_Ek = 0; // 上次偏差值初始化
    sptr->Ek_sum = 0; // 上上次偏差值初始化
    sptr->I_Out = 0;
    sptr->OUT = 0;
}

/*
函数名称：set_pid_target(PID_IncTypeDef *pid, float temp_val)
函数功能：设置目标值
函数变量：*sptr：pid参数结构体
函数返回： 无
例子：
*/
void set_pid_target(PID_IncTypeDef *pid, float temp_val)
{
    pid->target_val = temp_val; // 设置当前的目标值
}

/*
函数名称：get_pid_target(PID_IncTypeDef *pid)
函数功能：设置pid参数
函数变量：*sptr：pid参数结构体
函数返回： pid->target_val 当前目标值
例子：
*/
float get_pid_target(PID_IncTypeDef *pid)
{
    return pid->target_val; // 获取当前的目标值
}

/*
函数名称：set_p_i_d(PID_IncTypeDef *pid, float p, float i, float d)
函数功能：设置pid参数
函数变量：*sptr：pid参数结构体
例子：
*/
void set_p_i_d(PID_IncTypeDef *pid, float p, float i, float d)
{
    pid->Kp = p; // 设置比例系数 P
    pid->Ki = i; // 设置积分系数 I
    pid->Kd = d; // 设置微分系数 D
}

/***********************************************
* @brief : 变速积分 位置式PID
* @param : *PID         PID结构体
*          SetValue     设定值
*          ActualValue  实际值
*          Max_I        积分限幅
*          Coef_A       自行查询变速积分A与B的含义
*          Coef_B
* @return: float
* @date  : 2025年5月11日15:38:59
* @author: SJX
************************************************/
float Changing_Integration_Rate_Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I, float Coef_A, float Coef_B)
{
    PID->Ek = SetValue - ActualValue;
    float I_Term = PID->Ki * PID->Ek;
    if(PID->Ek * PID->I_Out > 0)
    {
        if(fabs(PID->Ek) <= Coef_B);       //不做处理
        else if(fabs(PID->Ek) <= (Coef_A + Coef_B))
            I_Term *= (Coef_A - fabs(PID->Ek) + Coef_B) / Coef_A;
        else
            I_Term = 0;
    }
    PID->I_Out += I_Term;

    if (PID->I_Out > Max_I)
        PID->I_Out = Max_I;
    if (PID->I_Out < -Max_I)
        PID->I_Out = -Max_I;

    PID->P_Out = (PID->Kp * PID->Ek);
    if(PID->OLS_Order > 2);         //稍后补充
    else
        PID->D_Out = PID->Kd * (PID->Ek - PID->last_Ek);

    PID->last_Ek = PID->Ek;

    PID->OUT = PID->P_Out + PID->I_Out + PID->D_Out;
    return PID->OUT;
}

//float Speed_Changing_Integration_Rate_Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I, float Coef_A, float Coef_B)
//{
//    PID->Ek = SetValue - (motor_speed_l + motor_speed_r);
//    float I_Term = PID->Ki * PID->Ek;
//    if(PID->Ek * PID->I_Out > 0)
//    {
//        if(fabs(PID->Ek) <= Coef_B);       //不做处理
//        else if(fabs(PID->Ek) <= (Coef_A + Coef_B))
//            I_Term *= (Coef_A - fabs(PID->Ek) + Coef_B) / Coef_A;
//        else
//            I_Term = 0;
//    }
//    PID->I_Out += I_Term;
//
//    if (PID->I_Out > Max_I)
//        PID->I_Out = Max_I;
//    if (PID->I_Out < -Max_I)
//        PID->I_Out = -Max_I;
//
//    PID->Ek = SetValue - ActualValue;
//
//    PID->P_Out = (PID->Kp * PID->Ek);
//    if(PID->OLS_Order > 2);         //稍后补充
//    else
//        PID->D_Out = PID->Kd * (PID->Ek - PID->last_Ek);
//
//    PID->last_Ek = PID->Ek;
//
//    PID->OUT = PID->P_Out + PID->I_Out + PID->D_Out;
//    return PID->OUT;
//}

