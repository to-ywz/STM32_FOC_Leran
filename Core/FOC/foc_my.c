#include "stdio.h"
#include "math.h"
#include "encoder.h"
#include "foc_my.h"
#include "stm32f4xx_hal.h"


#define PI 3.14159265358f
#define _3PI_2 4.71238898038f
#define PWM_PERIOD 5600
#define _constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

float voltage_power_supply;
float Ualpha, Ubeta = 0, Ua = 0, Ub = 0, Uc = 0;
float zero_electric_angle = 0;
int PP = 7, DIR = 1;

void DFOC_Vbus(float power_supply)
{
	voltage_power_supply = power_supply;
}

/**
 * @brief 将角度收拢到 【0，2Π】
 *
 * @param angle
 * @retval
 */
float _normalizeAngle(float angle)
{
	float a = fmod(angle, 2 * PI); // 取余运算可以用于归一化，列出特殊值例子算便知
	return a >= 0 ? a : (a + 2 * PI);
	// 三目运算符。格式：condition ? expr1 : expr2
	// 其中，condition 是要求值的条件表达式，如果条件成立，则返回 expr1 的值，否则返回 expr2 的值。可以将三目运算符视为 if-else 语句的简化形式。
	// fmod 函数的余数的符号与除数相同。因此，当 angle 的值为负数时，余数的符号将与 _2PI 的符号相反。也就是说，如果 angle 的值小于 0 且 _2PI 的值为正数，则 fmod(angle, _2PI) 的余数将为负数。
	// 例如，当 angle 的值为 -PI/2，_2PI 的值为 2PI 时，fmod(angle, _2PI) 将返回一个负数。在这种情况下，可以通过将负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
}

void setPwm(float Ua, float Ub, float Uc)
{

	// 计算占空比
	// 限制占空比从0到1
	float dc_a = _constrain(Ua / voltage_power_supply, 0.0f, 1.0f);
	float dc_b = _constrain(Ub / voltage_power_supply, 0.0f, 1.0f);
	float dc_c = _constrain(Uc / voltage_power_supply, 0.0f, 1.0f);

	// 写入PWM到PWM 0 1 2 通道
	TIM1->CCR1 = dc_a * 5600;
	TIM1->CCR2 = dc_b * 5600;
	TIM1->CCR3 = dc_c * 5600;
}

float setTorque(float Uq, float angle_el)
{
	Uq = _constrain(Uq, -voltage_power_supply / 2, voltage_power_supply / 2);
	float Ud = 0;
	angle_el = _normalizeAngle(angle_el);
	// 帕克逆变换
	Ualpha = -Uq * sin(angle_el);
	Ubeta = Uq * cos(angle_el);

	// 克拉克逆变换
	Ua = Ualpha + voltage_power_supply / 2;
	Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
	Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_power_supply / 2;
	setPwm(Ua, Ub, Uc);
	
	return 0;
}

float _electricalAngle()
{
	return _normalizeAngle((float)(DIR * PP) * encoder_ge_angle_without_trace() - zero_electric_angle);
}

void DFOC_alignSensor(int _PP, int _DIR)
{
	PP = _PP;
	DIR = _DIR;
	setTorque(3, _3PI_2);
	HAL_Delay(3000);
	zero_electric_angle = _electricalAngle();
	setTorque(0, _3PI_2);
	printf("0电角度：%.2f\r\n", zero_electric_angle);
}

float DFOC_M0_Angle()
{
	return encoder_get_polar_angle();
}
