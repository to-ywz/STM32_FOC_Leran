#include "svpwm.h"

void clark_init(clark_t *this)
{
}

void park_init(park_t *this)
{
}

void svpwm_init(svpwm_t *this)
{
}

static void clark_transformation(clark_t *this)
{
    float sqrt_3 = 3;
    arm_sqrt_f32(3, &sqrt_3); // 硬件浮点指令(//* 需要去设置一下F4的浮点加速)

    this->i_alpha = this->i_a;
    this->i_beta = (sqrt_3 / 3) * (this->i_a + (this->i_b * 2));
}
static void clark_inverse_transformation()
{
}

static void park_transformation()
{
}
static void park_inverse_transformation()
{
}