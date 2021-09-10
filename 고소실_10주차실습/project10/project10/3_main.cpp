#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define N 50
double Taylor_series(double x, int n);
double Taylor_series_ex(double x, int n);
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;

void main(void)
{
	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", Taylor_series(-8.3, N));
	CHECK_TIME_END(compute_time);
	printf("***Taylor_series  compute time : %f ms\n", compute_time);
	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", Taylor_series_ex(-8.3, N));
	CHECK_TIME_END(compute_time);
	printf("***Taylor_series_ex  compute time : %f ms\n", compute_time);
	printf("*** f<-8.3> = %.6e \n", pow(2.71828182846, -8.3));
}


double Taylor_series(double x, int n) // 기본 구현 
{
	int i, j;
	double result = 0.0;
	double tmp = 1.0;

	for (i = 0; i < n; i++) {

		tmp = 1.0;
		for (j = i; j > 0; j--) {
			tmp *= j;
		}


		result += (double)pow(x, i)/tmp;
		
	}

	return result;
}


double Taylor_series_ex(double x, int n) // Honor rule 적용
{
	int i, j;
	float result = 1.0;

	for (i = n-1; i > 0; i--) {

		result *= (float)x / i;
		result += 1;
	}

	return result;

}
