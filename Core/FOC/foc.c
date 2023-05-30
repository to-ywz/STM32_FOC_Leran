#include "stdint.h"
#include "foc.h"
#include "encoder.h"
#include "encoder.h"
#include "math.h"
#include "stm32f4xx_hal.h"
// #include <svpwm.h>

#define PI 3.14159265358f
#define _3PI_2 4.71238898038f
#define PWM_PERIOD 5600

// 钳位
#define CONSTRAIN(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

static float power_supply_volt = 0;
motor_info_t motor_0;

typedef struct
{
    // 输入
    float u_q;

    // 输出
    float u_alpha;
    float u_beta;

} park_t;

typedef struct
{
    // 输入
    float u_alpha;
    float u_beta;

    float u_a;
    float u_b;
    float u_c;
} clark_t;

static void set_pwm_duty(float u_a, float u_b, float u_c);
static float normalize_angle(float);

static float normalize_angle(float angle)
{
    float a = fmod(angle, 2 * PI);
    return a >= 0 ? a : (a + 2 * PI);
}

/**
 * @brief 传感器矫正
 * 
 * @param _PP:  
 * @param _DIR: 
 */
void foc_align_sensor(int _PP, int _DIR)
{
    motor_0.PP = _PP;
    motor_0.DIR = _DIR;
    setTorque(3, _3PI_2);
    HAL_Delay(3000);
    motor_0.zero_electric_angle = _electricalAngle();
    setTorque(0, _3PI_2);
    printf("0电角度：%.2f\r\n", motor_0.zero_electric_angle);
}

static void electrical_angle(void)
{
    return _normalizeAngle((float)(motor_0.DIR * motor_0.PP) * encoder_ge_angle_without_trace() - motor_0.zero_electric_angle);
}

static void set_pwm_duty(float u_a, float u_b, float u_c)
{
    // 计算占空比
    float duty_a = CONSTRAIN(u_a / 1, 0.0f, 1);
    float duty_b = CONSTRAIN(u_a / 1, 0.0f, 1);
    float duty_c = CONSTRAIN(u_a / 1, 0.0f, 1);

    TIM1->CCR1 = duty_a * PWM_PERIOD;
    TIM1->CCR2 = duty_b * PWM_PERIOD;
    TIM1->CCR3 = duty_c * PWM_PERIOD;
}

// END OF FILE
