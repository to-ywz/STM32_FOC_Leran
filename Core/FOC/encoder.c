#include "encoder.h"
#define abs(x) ((x) > 0 ? (x) : -(x))
#define PI 3.14159265358
#define _2PI 6.28318530718

static float angle_data_prev;      // 上次位置
static float full_rotation_offset; // 转过的整圈数
static int32_t full_rotations = 0;

static uint16_t bsp_as5600GetRawAngle(void);

void bsp_as5600Init(void)
{
  /* init i2c interface */

  /* init var */
  full_rotation_offset = 0;
  angle_data_prev = bsp_as5600GetRawAngle();
}

static int i2cWrite(uint8_t dev_addr, uint8_t *pData, uint32_t count)
{
  int status;
  int i2c_time_out = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;

  status = HAL_I2C_Master_Transmit(&AS5600_I2C_HANDLE, dev_addr, pData, count, i2c_time_out);
  return status;
}

static int i2cRead(uint8_t dev_addr, uint8_t *pData, uint32_t count)
{
  int status;
  int i2c_time_out = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;

  status = HAL_I2C_Master_Receive(&AS5600_I2C_HANDLE, (dev_addr | 1), pData, count, i2c_time_out);
  return status;
}

uint16_t bsp_as5600GetRawAngle(void)
{
  uint16_t raw_angle = 0;
  uint8_t buffer[2] = {0};
  uint8_t raw_angle_register = AS5600_RAW_ANGLE_REGISTER;

  i2cWrite(AS5600_ADDR, &raw_angle_register, 1);
  i2cRead(AS5600_ADDR, buffer, 2);
  raw_angle = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
  return raw_angle;
}

float bsp_as5600GetAngle_without_trak()
{
  return bsp_as5600GetRawAngle() * 0.08789 * PI / 180; // 得到弧度制的角度
}

float bsp_as5600GetAngle(void)
{
  float angle_data = bsp_as5600GetRawAngle();

  float d_angle = angle_data - angle_data_prev;
  if (abs(d_angle) > (0.8 * AS5600_RESOLUTION))
  {
    full_rotation_offset += (d_angle > 0 ? -_2PI : _2PI);
  }
  angle_data_prev = angle_data;

  return (full_rotation_offset + (angle_data / (float)AS5600_RESOLUTION) * _2PI);
}

float bsp_as5600_get_polar_angle(void)
{
  float val = bsp_as5600GetAngle_without_trak();
  static float p_angle = 0;
  float d_angle = val - p_angle;

  if (abs(d_angle) > (0.8f * 6.28318530718f))
    full_rotations += (d_angle > 0) ? -1 : 1;
  p_angle = val;

  return (float)full_rotations * 6.28318530718f + p_angle;
}

void encoder_init(void)
{
  bsp_as5600Init();
}

uint16_t encoder_get_raw_angle(void)
{
  return bsp_as5600GetRawAngle();
}

float encoder_ge_angle_without_trace(void)
{
  return bsp_as5600_get_polar_angle();
}

float encoder_get_angle(void)
{
  return bsp_as5600GetAngle();
}

float encoder_get_polar_angle(void)
{
  return bsp_as5600_get_polar_angle();
}