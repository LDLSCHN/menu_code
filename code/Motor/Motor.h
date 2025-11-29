/*
 * Motor.h
 *
 *  Created on: 2025年10月24日
 *      Author: LDLS
 */

#ifndef CODE_MOTOR_MOTOR_H_
#define CODE_MOTOR_MOTOR_H_

#include "MyHeadFile.h"

void Motor_Init(void);
void Speed_PIDL(void);
void Speed_PIDR(void);
void Control_Speed(void);
void Speed_Measure(void);
void Get_Speed_L(void);
void Get_Speed_R(void);
void Motor_stop(void);

//typedef struct
//{
//      uint8 Motor_status;
//      uint8 Steer_status;
//}Car_Statustypedef;

//extern Car_Statustypedef Carstatus;
extern int16 speed1, speed2;
extern int16 Speed_Goal;
extern int16 set_speed;

extern int16 Speed_Goal_l;
extern int16 Speed_Goal_r;

extern int16 Speed_PID_OUT_l;
extern int16 Speed_PID_OUT_r;

extern int16 Speed_Encoder_l;
extern int16 Speed_Encoder_r;

extern int16 L_ring_speed_fast;
extern int16 L_ring_speed_slow;
extern int16 R_ring_speed_fast;
extern int16 R_ring_speed_slow;


#endif /* CODE_MOTOR_MOTOR_H_ */
