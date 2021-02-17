#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
	float r;
	float i;
	float j;
	float k;
} Quaternion;

Quaternion QFromEuler(float roll, float pitch, float yaw);//angles are in degrees
void QMultiply(Quaternion *Q1, Quaternion *Q2, Quaternion* res);
void QToEuler(Quaternion* Q, float* res);
void QConjugate(Quaternion* Q);
void QNormalize(Quaternion* Q);
void QPrint(Quaternion* Q);
#ifdef __cplusplus
}
#endif
