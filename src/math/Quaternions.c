#define M_PIff 3.14159265358979323846f
#define degToRad  M_PIff / 180.f
#define radToDeg 180.f/M_PIff

#include <math.h>
#include <stdio.h>
#include"Quaternions.h"




void QPrint(Quaternion *Q){
    printf("%f %f %f %f",Q->r,Q->i,Q->j,Q->k);
}

Quaternion QFromEuler(float roll, float pitch, float yaw) {

    float cr = cosf(roll*0.5f);
    float sr = sinf(roll*0.5f);
    float cp = cosf(pitch*0.5f);
    float sp = sinf(pitch*0.5f);
    float cy = cosf(yaw*0.5f);
    float sy = sinf(yaw*0.5f);

    Quaternion ret;
    ret.r = cr * cp * cy + sr * sp * sy;
    ret.i = sr * cp * cy - cr * sp * sy;
    ret.j = cr * sp * cy + sr * cp * sy;
    ret.k = cr * cp * sy - sr * sp * cy;
    return ret;
}
void QMultiply(Quaternion *Q1, Quaternion *Q2, Quaternion* res) {
   float r = (Q1->r * Q2->r - Q1->i * Q2->i - Q1->j * Q2->j - Q1->k * Q2->k);
   float i  = (Q1->r * Q2->i + Q2->r * Q1->i + Q1->j * Q2->k - Q1->k * Q2->j);
   float j = (Q1->r * Q2->j - Q1->i * Q2->k + Q1->j * Q2->r + Q1->k * Q2->i);
   float k  = (Q1->r * Q2->k + Q1->i * Q2->j - Q1->j * Q2->i + Q1->k * Q2->r);
   res->r = r;
   res->i = i;
   res->j = j;
   res->k = k;
}

void QToEuler(Quaternion* Q, float* res) {
    //res is roll pitch yaw
    float Qr2 = Q->r * Q->r;
    float Qi2 = Q->i * Q->i;
    float Qj2 = Q->j * Q->j;
    float Qk2 = Q->k * Q->k;

    res[0] = atan2f(2.f * (Q->j * Q->k + Q->r + Q->i), Qr2 - Qi2 - Qj2 + Qk2);
    res[1] = asinf(-2.f * (Q->i * Q->k - Q->r * Q->j));
    res[2] = atan2f(2.f * (Q->i*Q->j + Q->r *Q->k), (Qr2 + Qi2 + Qj2 + Qk2));
}
void QConjugate(Quaternion* Q) {
    Q->i *= -1.f;
    Q->j *= -1.f;
    Q->k *= -1.f;
}
void QNormalize(Quaternion* Q) {
    float lenght = sqrtf(Q->r * Q->r + Q->i * Q->i + Q->j * Q->j + Q->k * Q->k);
    Q->r /= lenght;
    Q->i /= lenght;
    Q->j /= lenght;
    Q->k /= lenght;
}