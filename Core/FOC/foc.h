#ifndef FOC_H_
#define FOC_H_

typedef struct
{
    uint8_t PP;
    uint8_t DIR;
    float voltage_power_supply;
    float zero_electric_angle;
} motor_info_t;

extern motor_info_t motor_0;

#endif
