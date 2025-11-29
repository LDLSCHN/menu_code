/*
 * Status.h
 *
 *  Created on: 2025年11月14日
 *      Author: LDLS
 */

#ifndef CODE_STATUS_STATUS_H_
#define CODE_STATUS_STATUS_H_

#include "MyHeadFile.h"

typedef enum
{
    status_car_stop = 0,
    status_car_start = 1,
}Car_Status;

typedef enum
{
    status_steer_stop = 0,
    status_steer_start = 1,
}Steer_Status;

typedef enum
{
    status_R_round_off = 0,
    status_R_round_on = 1,
}R_Round_Status;
typedef enum
{
    status_L_round_off = 0,
    status_L_round_on = 1,
}L_Round_Status;
typedef enum
{
    status_cross_off = 0,
    status_cross_on = 1,
}cross_Status;


extern uint16 g_Car_Status;
extern uint16 Steer_status;
extern uint16 R_round_status;
extern uint16 L_round_status;
extern uint16 cross_status;

void Car_Start(void);
void Car_Stop(void);
void Steer_activate(void);
void Steer_off(void);



#endif /* CODE_STATUS_STATUS_H_ */
