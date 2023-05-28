#ifndef _SVPWM_H__
#define _SVPWM_H__

#include <stdint.h>

// clark 变换结构体
typedef struct clark
{
    // 输入参数
    uint32_t i_a; // 输入 A 相定子电流
    uint32_t i_b; // 输入 B 相定子电流
    uint32_t i_c; // 输入 C 相定子电流

    // 输出
    uint32_t i_alpha; // 静止坐标系 Iα 电流
    uint32_t i_beta;  // 静止坐标系 Iβ 电流

    // 方法
    void (*clark_transformation)();         // Clark 变化
    void (*clark_inverse_transformation)(); // Clark 逆变换

} clark_t;

// park 变换结构体
typedef struct park
{
    // 输出
    float u_q;    //  输出 旋转坐标系 D坐标值电压
    float u_d;    //  输出 旋转坐标系 Q坐标值电压
    uint32_t i_d; // 输出 旋转坐标系 D坐标值电流
    uint32_t i_q; // 输出 旋转坐标系 Q坐标值电流

    uint32_t i_alpha; // 输入, 静止坐标系 Iα 电流
    uint32_t i_beta;  // 输入, 静止坐标系 Iβ 电流

    // 输入参数
    float u_alpha; // 静止坐标系 Iα 电压
    float u_beta;  // 静止坐标系 Iβ 电压    `

    // 固有参数
    uint32_t theta; // 旋转坐标角度

    // 检测参数
    float act_id; // 实际D轴电流
    float act_iq; // 实际I轴电流

    void (*park_transformation)();         // park 变换
    void (*park_inverse_transformation)(); // park 变换
} park_t;

// svpwm 结构体
typedef struct svpwm
{
    float u_alpha; // 静止坐标系 Iα 电压
    float u_beta;  // 静止坐标系 Iβ 电压

    uint32_t u_a; //
    uint32_t u_b; //
    uint32_t u_c; //

    uint32_t t_x;
    uint32_t t_y;
    uint32_t t_z;

    // 一个SVPWM 的生产分为 三个相位 各自的PWM时间
    uint32_t ta_on; // A 相位时间
    uint32_t tb_on; // B 相位时间
    uint32_t tc_on; // C 相位时间

} svpwm_t;

#endif // _SVPWM_H__
