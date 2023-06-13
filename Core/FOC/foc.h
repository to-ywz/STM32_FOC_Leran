#ifndef FOC_H_
#define FOC_H_

#include "stdint.h"

#define PWM_PERIOD 5600
#define _FOC_COMSTRAIN(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

typedef struct
{
    float motor_power; // 电机电压

    uint8_t poles; // 磁极对

    uint16_t speed; // 转速

} motor_t;

void foc_init(float power, uint8_t pp, int8_t dir);
void foc_align_sensor(void);
void foc_set_torque(float u_q, float angle_el);
float get_electrical_angle(void);

#endif
