#include "stdint.h"
#include "foc.h"

// #include <svpwm.h>

#define PI 3.14159265358f
#define _3PI_2 4.71238898038f
#define PWM_PERIOD 5600

// 钳位
#define _constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

static float power_supply_volt = 0;

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

// END OF FILE
