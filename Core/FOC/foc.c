#include "stdio.h"
#include "math.h"
#include "encoder.h"
#include "foc.h"
#include "stm32f4xx_hal.h"
// #include <svpwm.h>

#define PI 3.14159265358f
#define _3PI_2 4.71238898038f
#define PWM_PERIOD 5600

// 钳位
#define CONSTRAIN(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

motor_t motor;
static int8_t s_dir;
static float s_ele_zero;
static float Ualpha, Ubeta = 0, Ua = 0, Ub = 0, Uc = 0;

static void setPwm(float Ua, float Ub, float Uc);



static float _normalizeAngle(float angle)
{
    float a = fmod(angle, 2 * PI); // 取余运算可以用于归一化，列出特殊值例子算便知
    return a >= 0 ? a : (a + 2 * PI);
}

float get_electrical_angle(void)
{
    return _normalizeAngle((float)(s_dir * motor.poles) * encoder_ge_angle_without_trace() - s_ele_zero);
}

static void setPwm(float Ua, float Ub, float Uc)
{

    // 计算占空比
    // 限制占空比从0到1
    float dc_a = _FOC_COMSTRAIN(Ua / motor.motor_power, 0.0f, 1.0f);
    float dc_b = _FOC_COMSTRAIN(Ub / motor.motor_power, 0.0f, 1.0f);
    float dc_c = _FOC_COMSTRAIN(Uc / motor.motor_power, 0.0f, 1.0f);

    // 写入PWM到PWM 0 1 2 通道
    TIM1->CCR1 = dc_a * 5600;
    TIM1->CCR2 = dc_b * 5600;
    TIM1->CCR3 = dc_c * 5600;
}

void foc_set_torque(float u_q, float angle_el)
{
    float u_d = 0;
    u_q = _FOC_COMSTRAIN(u_q, -motor.motor_power / 2, motor.motor_power / 2);

    // 帕克逆变换
    Ualpha = -u_q * sin(angle_el);
    Ubeta = u_d * cos(angle_el);

    // 克拉克逆变换
    Ua = Ualpha + -motor.motor_power / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + -motor.motor_power / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + -motor.motor_power / 2;
    setPwm(Ua, Ub, Uc);
}

/**
 * @brief foc 初始化
 *
 * @param power: 电机电源
 * @param pp: 磁极对
 * @param dir: 传感器方向
 */
void foc_init(float power, uint8_t pp, int8_t dir)
{
    motor.poles = pp;
    motor.motor_power = power;
    s_dir = dir;
}

/**
 * @brief 传感器对齐
 *
 */
void foc_align_sensor(void)
{
    foc_set_torque(3, _3PI_2);
    HAL_Delay(3000);

    s_ele_zero = get_electrical_angle();
    foc_set_torque(0, _3PI_2);
    printf("zero_electrical_angle:%.2f\r\n", s_ele_zero);
}

// END OF FILE
