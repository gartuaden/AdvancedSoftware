#define _USE_MATH_DEFINES
#include <math.h>

double _f1(double x) {
	return (x*x*x - 2*x*x + x - 3);
}

double _fp1(double x) {
	return (3*x*x - 4*x + 1);
}

// f1(x)
double _f2(double x) {
	return (x * x - 4*x + 4 - log(x));
}

double _fp2(double x) {
	return (2*x - 4 - 1/x);
}


// f2(x)
double _f3(double x) {
	return (x + 1 - 2*sin(M_PI*x));
}

double _fp3(double x) {
	return (1 - 2*M_PI*cos(M_PI*x));
}

// f3(x)
double _f4(double x) {
	return (x*x*x*x - 11.0*x*x*x + 42.35*x*x - 66.55*x + 35.1384);
}

double _fp4(double x) {
	return (4 * x * x * x - 33.0 * x * x + 84.7 * x - 66.55);
}

double _f5(double x) {
	return (log(x) - 1);
}

double _fp5(double x) {
	return (1 / x);
}

double _fA(double x) {

	double l, h, D, beta;
	double A, B, C, E;
	l = 89;
	h = 49;
	D = 55;
	beta = 11.5;

	A = l * sin(beta);
	B = l * cos(beta);
	C = (h + 0.5 * D) * sin(beta) - 0.5 * D * tan(beta);
	E = (h + 0.5 * D) * cos(beta) - 0.5 * D;

	return A * sin(x) * cos(x) + B * sin(x) * sin(x) - C * cos(x) - E * sin(x);
}

double _fpA(double x) {

	double l, h, D, beta;
	double A, B, C, E;
	l = 89;
	h = 49;
	D = 55;
	beta = 11.5;

	A = l * sin(beta);
	B = l * cos(beta);
	C = (h + 0.5 * D) * sin(beta) - 0.5 * D * tan(beta);
	E = (h + 0.5 * D) * cos(beta) - 0.5 * D;

	return A * (cos(x) * cos(x) - sin(x) * sin(x)) + B * 2 * sin(x) * cos(x) + C * sin(x) - E * cos(x);
}
