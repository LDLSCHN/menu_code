/*
 * Status.c
 *
 *  Created on: 2025年11月14日
 *      Author: LDLS
 */
#include "Status.h"

uint16 g_Car_Status = status_car_stop;
uint16 Steer_status = status_steer_stop;
uint16 R_round_status = status_R_round_on;
uint16 L_round_status = status_L_round_on;
uint16 cross_status = status_cross_on;

void Car_Start(void)
{
    system_delay_ms(200);
    Beep_ShortRing();
    PID_clear(&Steer_PID);
    PID_clear(&Motor_Speed_PID_l);
    PID_clear(&Motor_Speed_PID_r);
    PID_clear(&Turn_PID);
    L_roundabout_state = 0;
    R_roundabout_state = 0;
    cross_state = 0;
    g_Car_Status = status_car_start;
    Steer_status = status_steer_start;
}

void Car_Stop(void)
{
    g_Car_Status = status_car_stop;
    Steer_status = status_steer_stop;
//    Speed_Goal_l = 0;
//    Speed_Goal_r = 0;
}

void Steer_activate(void)
{
    Steer_status = status_steer_start;
}

void Steer_off(void)
{
    Steer_status = status_steer_stop;
}


