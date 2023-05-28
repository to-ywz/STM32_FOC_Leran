#ifndef _FOC_MY_H
#define _FOC_MY_H

#define PI 3.14159265358f

void setPwm(float Ua, float Ub, float Uc);
float setTorque(float Uq, float angle_el);
float _normalizeAngle(float angle);
void DFOC_Vbus(float power_supply);
void DFOC_alignSensor(int _PP, int _DIR);
float _electricalAngle();
float DFOC_M0_Angle();

#endif