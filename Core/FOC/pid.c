/**
 * @file pid.c
 * @author BlackSheep (blacksheep.208h@gmail.com)
 * @brief pid
 * @version 0.1
 * @date 2023-06-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "pid.h"

/**
 * @brief pid 初始化
 *
 * @param pid  实例
 * @param kp    比例系数
 * @param ki    比例系数
 * @param kd    微分系数
 * @param setpoint 设定值(目标值)
 */
void pid_init(pid_controller_t *pid, float kp, float ki, float kd, float setpoint)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = setpoint;
    pid->prev_error = 0;
    pid->integral = 0;
}

/**
 * @brief   pid 数据更新
 *
 * @param pid   实力
 * @param input 当前测量值
 * @param dt    间隔时间(s)
 * @retval      控制器值
 */
float pid_update(pid_controller_t *pid, float input, float dt)
{
    float error = pid->setpoint - input;
    pid->integral += error * dt;
    float derivative = (error - pid->prev_error) / dt;
    float output = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    pid->prev_error = error;
    return output;
}
