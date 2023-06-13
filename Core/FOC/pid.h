#ifndef PID_H_
#define PID_H_

typedef struct
{
    float kp;         // 比例系数
    float ki;         // 积分系数
    float kd;         // 微分系数
    float setpoint;   // 目标值
    float prev_error; // 上次误差
    float integral;   // 累积误差
} pid_controller_t;

void pid_init(pid_controller_t *pid, float kp, float ki, float kd, float setpoint);
float pid_update(pid_controller_t *pid, float input, float dt);

#endif
