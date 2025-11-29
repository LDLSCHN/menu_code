#ifndef PID_H
#define PID_H

#include "zf_common_typedef.h"
#include "MyHeadfile.h"

typedef struct {
    float target_val;             //Ŀ��ֵ���൱��SetValue
    float Kp;                       //����ϵ��Proportional
    float Ki;                       //����ϵ��Integral
    float Kd;                       //΢��ϵ��Derivative
    float Ek;                       //��ǰ���
    float last_Ek;                      //ǰһ����� e(k-1)
    float Ek_sum;                      //������
    float OUT;                      //PID���
    float OUT1;
    float P_Out;
    float I_Out;
    float D_Out;
    float I_Max;
    unsigned short OLS_Order;               //最小二乘法求微分样本数
} PID_IncTypeDef;

extern PID_IncTypeDef Motor_Speed_PID_l;
extern PID_IncTypeDef Motor_Speed_PID_r;
extern PID_IncTypeDef Steer_PID;
extern PID_IncTypeDef Turn_PID;

/*****************************************************函数部分**************************************************************/
void PID_Inc_Init(PID_IncTypeDef *sptr, float kp, float ki, float kd, float i_max);
float Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I);
void PID_clear(PID_IncTypeDef *sptr);
void PID_param_init(void);
void set_pid_target(PID_IncTypeDef *pid, float temp_val);
float get_pid_target(PID_IncTypeDef *pid);
void set_p_i_d(PID_IncTypeDef *pid, float p, float i, float d);
float Incremental_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue);
float Changing_Integration_Rate_Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I, float Coef_A, float Coef_B);
float Speed_Changing_Integration_Rate_Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I, float Coef_A, float Coef_B);

#endif
