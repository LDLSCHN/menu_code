/*
 * Steer.h
 *
 *  Created on: 2025年10月25日
 *      Author: LDLS
 */

#ifndef CODE_STEER_STEER_H_
#define CODE_STEER_STEER_H_

#include "MyHeadFile.h"

#define Steer_MID  5295
#define Steer_right  4725            //舵机右打
#define Steer_left   5815             //舵机左打

extern int16 steer_debug_pwm;

void Steer_Init(void);
void Steer_PD(void);
void Steer_Control(uint32 duty);  //舵机控制

#endif /* CODE_STEER_STEER_H_ */
