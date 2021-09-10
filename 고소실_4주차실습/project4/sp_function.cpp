#define _USE_MATH_DEFINES
#include <math.h>
float _sp_f1(float x) {
	return (x * x * x - 2 * x * x + x - 3);
}

float _sp_fp1(float x) {
	return (3 * x * x - 4 * x + 1);
}


float _sp_f2(float x) {
	return (x * x - 4 * x + 4 - log(x));
}

float _sp_fp2(float x) {
	return (2 * x - 4 - 1 / x);
}

float _sp_f3(float x) {
	return (x + 1 - 2 * sin(M_PI * x));
}

float _sp_fp3(float x) {
	return (1 - 2 * M_PI * cos(M_PI * x));
}


float _sp_f4(float x) {
	return (x * x * x * x - 11.0 * x * x * x + 42.35 * x * x - 66.55 * x + 35.1384);
}

float _sp_fp4(float x) {
	return (4 * x * x * x - 33.0 * x * x + 84.7 * x - 66.55);
}

float _sp_f5(float x) {
	return (log(x) - 1);
}

float _sp_fp5(float x) {
	return (1/x);
}
