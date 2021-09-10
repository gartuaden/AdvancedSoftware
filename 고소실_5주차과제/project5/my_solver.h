#ifndef __MY_SOLVER_H__
#define __MY_SOLVER_H__

#include <stdio.h>
#include <Windows.h>

#define CHECK_TIME_START QueryPerformanceFrequency((_LARGE_INTEGER*) &freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);

#define DELTA 0.000000000001
#define Nmax 100
#define EPSILON 0.00000000001


static unsigned long int next;
#define MY_RAND_MAX 32767


void program2_1();
void program2_2();
double fx(double xn,double seed, double *x, double *y, int n);

double _f(double xn, double seed, double* x, double* y, int n);
double _fp(double xn, double seed, double* x, double* y, int n);

// HOMEWORK
void program2_2_a(void);
void program2_2_b(void);
void program2_3(void);

void function2_1(void);

#endif  // __MY_SOLVER_H__
