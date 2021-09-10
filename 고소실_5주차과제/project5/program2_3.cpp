#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>


// HOMEWORK 2-1

void function2_1() {

	int nr;
	double* U, * X_a, *X_b, *X_c, tmp; // random numbers
	double iseed, ramda_a, ramda_b, ramda_c;
	double average = 0, avg_sum = 0, devi_sum = 0, devi = 0, var = 0;

	srand(time(NULL));
	printf("생성할 지수 분포를 따르는 난수 개수를 입력하시오: ");
	scanf("%d", &nr);

	U = (double*)malloc(sizeof(double) * nr);
	X_a = (double*)malloc(sizeof(double) * nr);
	X_b = (double*)malloc(sizeof(double) * nr);
	X_c = (double*)malloc(sizeof(double) * nr);

	// Generate random numbers between 0 and 1
	for (int i = 0; i < nr; i++) {
		iseed = (double)rand() / (RAND_MAX + 1);
		U[i] = iseed;
		//printf("U[%d]: %.15lf\n", i, U[i]);
	}


	// Allocate three different ramda values which are greater than 0
	ramda_a = 0.5;
	ramda_b = 1.0;
	ramda_c = 1.5;

	// Calculate X that follows each exponential distribution
	for (int i = 0; i < nr; i++) {
		X_a[i] = -(log(1 - U[i]) / ramda_a);
		X_b[i] = -(log(1 - U[i]) / ramda_b);
		X_c[i] = -(log(1 - U[i]) / ramda_c);
	}



	// Calculate the average and variance for comparing with theratical values
	// Ramda_a
	for (int i = 0; i < nr; i++) {
		avg_sum += X_a[i];
	}
	average = avg_sum / (double)nr;

	for (int i = 0; i < nr; i++) {
		devi = X_a[i] - average;
		devi_sum += devi * devi;
	}
	
	var = devi_sum / (double)nr;
	printf("-------------------------------------------------------------------------\n");
	printf("ram = %lf		1/ram = %lf		1/ram^2 = %lf\n", ramda_a, 1 / ramda_a, 1 / (ramda_a * ramda_a));
	printf("			E[X]= %lf			Var[X]=%lf\n", average, var);
	printf("-------------------------------------------------------------------------\n");

	avg_sum = 0;
	devi_sum = 0;

	// Ramda_b
	for (int i = 0; i < nr; i++) {
		avg_sum += X_b[i];
	}
	average = avg_sum / (double)nr;

	for (int i = 0; i < nr; i++) {
		devi = X_b[i] - average;
		devi_sum += devi * devi;
	}

	var = devi_sum / (double)nr;
	printf("ram = %lf		1/ram = %lf		1/ram^2 = %lf\n", ramda_b, 1 / ramda_b, 1 / (ramda_b * ramda_b));
	printf("			E[X]= %lf			Var[X]=%lf\n", average, var);
	printf("-------------------------------------------------------------------------\n");
	avg_sum = 0;
	devi_sum = 0;

	// Rmada_c
	for (int i = 0; i < nr; i++) {
		avg_sum += X_c[i];
	}
	average = avg_sum / (double)nr;

	for (int i = 0; i < nr; i++) {
		devi = X_c[i] - average;
		devi_sum += devi * devi;
	}

	var = devi_sum / (double)nr;
	printf("ram = %lf		1/ram = %lf		1/ram^2 = %lf\n", ramda_c, 1 / ramda_c, 1 / (ramda_c * ramda_c));
	printf("			E[X]= %lf			Var[X]=%lf\n", average, var);
	printf("-------------------------------------------------------------------------\n");

	free(U);
	free(X_a);
	free(X_b);
	free(X_c);
}

// HOMEWORK
void program2_3()
{
	FILE* fp_r1, *fp_r2, * fp_w;

	fp_r1 = fopen("pdf_table.txt", "r");
	fp_r2 = fopen("random_event_table.txt", "r");
	fp_w = fopen("histogram.txt", "w");

	// variables for reading
	int nr, n;
	double h;
	double* x, * y, *xr, tmp;

	int current = 0, xr_count = 0;
	double sect_point, sect_interval;


	// reading from text files
	fscanf(fp_r1, "%d %lf", &n, &h);

	x = (double*)malloc(sizeof(double) * n);
	y = (double*)malloc(sizeof(double) * n);

	for (int i = 0; i < n; i++) {
		fscanf(fp_r1, "%lf %lf", &x[i], &y[i]);
	}
	
	fscanf(fp_r2, "%d", &nr);

	xr = (double*)malloc(sizeof(double) * nr);

	for (int i = 0; i < nr; i++) {
		fscanf(fp_r2, "%lf", &xr[i]);
	}

	// sort the xr
	for (int i = 0; i < nr; i++) {
		for (int j = 0; j < nr - i - 1; j++) {
			if (xr[j + 1] < xr[j]) {
				tmp = xr[j];
				xr[j] = xr[j + 1];
				xr[j + 1] = tmp;
			}
		}
	}


	// divide the section into 500
	sect_interval = 1 / (double)500;
	sect_point = x[0];

	for (int i = 0; i < 500; i++) {

		while (1) {
			// if the xr doesn't belong to the current section, break
			if (current >= nr || sect_point > xr[current] || xr[current] > sect_point + sect_interval) {
				break;
			}
			xr_count++;
			current++;
		}
		fprintf(fp_w, "Section[%.15lf-%.15lf]: The number of random numbers is %d, The probability is %.15f\n", sect_point, sect_point + sect_interval, xr_count, (double)xr_count/nr);

		// move to the next section
		sect_point += sect_interval;
		xr_count = 0;
	}

	free(x);
	free(y);
	free(xr);



	if (fp_r1 != NULL) fclose(fp_r1);
	if (fp_r2 != NULL) fclose(fp_r2);
	if (fp_w != NULL) fclose(fp_w);

	// HOMEWORK 2-1
	function2_1();

	program2_2_a();
	program2_2_b();

}

// HOMEWORK
void program2_2_a()
{
	int nr;
	int i = 0, j = 0;
	int n;
	double h;
	double* y, * x;
	double iseed;
	double xn, an, bn, an1, bn1;
	double* out_xn;

	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");
	//fp_w = fopen("temp1.txt", "w");


	srand(time(NULL));
	fscanf(fp_r, "%d %lf", &n, &h);

	x = (double*)malloc(sizeof(double) * n);
	y = (double*)malloc(sizeof(double) * n);


	for (i = 0; i < n; i++) {
		fscanf(fp_r, "%lf %lf", &x[i], &y[i]);
	}

	nr = 1000;
	fprintf(fp_w, "%d\n", nr);
	out_xn = (double*)malloc(sizeof(double) * nr);

	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;


	/***************************************************/
	// something to do...

	// Bisection Method
	for (i = 0; i < nr; i++) {
		iseed = (double)rand() / (RAND_MAX + 1);
		//	printf("iseed is %lf\n", iseed);
		an = 0.0;
		bn = 1.0;
		j = 0;

		while (1) {
			xn = (an + bn) / 2.0;

			if (_f(xn, iseed, x, y, n) == 0) {
				out_xn[i] = xn;
				break;
			}

			if (j >= Nmax || fabs(_f(xn, iseed, x, y, n)) < DELTA || (fabs(bn - an) < EPSILON)) {
				out_xn[i] = xn;
				break;
			}

			if (_f(an, iseed, x, y, n) * _f(xn, iseed, x, y, n) < 0) {
				bn1 = xn;
				an1 = an;
			}
			else {
				an1 = xn;
				bn1 = bn;
			}

			an = an1;
			bn = bn1;
			j++;
		}
	}

	/***************************************************/


	CHECK_TIME_END(resultTime);

	printf("The program2_2_a run time is %f(ms)..\n", resultTime * 1000.0);

	for (int i = 0; i < nr; i++) {
		fprintf(fp_w, "%.15lf\n", out_xn[i]);
	}


	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);


	free(x);
	free(y);
	free(out_xn);
}

void program2_2_b()
{
	int nr;
	int i = 0, j = 0;
	int n;
	double h;
	double* y, * x;
	double iseed;
	double xn, xnn, xn1, an, bn, an1, bn1, cn;
	double* out_xn;

	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");


	srand(time(NULL));
	fscanf(fp_r, "%d %lf", &n, &h);

	x = (double*)malloc(sizeof(double) * n);
	y = (double*)malloc(sizeof(double) * n);


	for (i = 0; i < n; i++) {
		fscanf(fp_r, "%lf %lf", &x[i], &y[i]);
	}

	nr = 1000;
	fprintf(fp_w, "%d\n", nr);
	out_xn = (double*)malloc(sizeof(double) * nr);

	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;

	// something to do...
	
	for (i = 0; i < nr; i++) {
		iseed = (double)rand() / (RAND_MAX + 1);

		// Getting x1 and x0 by using Bisection Method
		an = 0.0;
		bn = 1.0;
		for (int i = 0; i < 5; i++) {
			cn = (an + bn) / 2.0;
			if (_f(an, iseed, x, y, n) * _f(cn, iseed, x, y, n) < 0) {
				bn1 = cn;
				an1 = an;
			}
			else {
				an1 = cn;
				bn1 = bn;
			}
			an = an1;
			bn = bn1;
		}
		

		//**********************************************************//
		// Secant Method
		
		xnn = an; // xn-1
		xn = bn; // xn
		j = 0;

		while (1) {
			// xn+1
			xn1 = xn - _f(xn, iseed, x, y, n) * (xn - xnn) / (_f(xn, iseed, x, y, n) - _f(xnn, iseed, x, y, n));

			if (j >= Nmax || fabs(_f(xn1, iseed, x, y, n)) < DELTA || (fabs(xn1 - xn) < EPSILON)) {
				out_xn[i] = xn1;
				break;
			}

			xnn = xn;
			xn = xn1;
			j++;
		}

		//**********************************************************//

		//**********************************************************//
		// Newton Method
		/*
		xn = an;
		j = 0;
		while (1) {
			xn1 = xn - (_f(xn, iseed, x, y, n) / _fp(xn, iseed, x, y, n));
			if (j >= Nmax || fabs(_f(xn1, iseed, x, y, n)) < DELTA || (fabs(xn1 - xn) < EPSILON)) {
				out_xn[i] = xn1;
				break;
			}
			xn = xn1;
			j++;
		}
		*/
		//**********************************************************//

	}



	CHECK_TIME_END(resultTime);

	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);

	for (int i = 0; i < nr; i++) {
		fprintf(fp_w, "%.15lf\n", out_xn[i]);
	}


	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);


	free(x);
	free(y);
	free(out_xn);


}

double _f(double xn, double seed, double* x, double* y, int n) {
	double sum = 0;
	int max = 0;


	for (int i = 0; i < n; i++) {
		if (x[i] >= xn) {
			break;
		}
		max = i;
	}


	// 0~max 사다리꼴
	for (int i = 0; i < max + 1; i++) {
		if (i + 1 == max + 1) break;
		sum += (x[1] - x[0]) / 2 * (y[i + 1] + y[i]);
	}

	// 나머지 부분
	sum += (y[max] + (y[max + 1] - y[max]) / (x[max + 1] - x[max]) * (xn - x[max]) / 2.0) * (xn - x[max]) - seed;

	return sum;
}

double _fp(double xn, double seed, double* x, double* y, int n) {

	double s;
	int idx = 0;


	for (int i = 0; i < n; i++) {
		if (x[i] >= xn) {
			break;
		}
		idx = i;
	}

	s = (xn - x[idx]) / (x[idx + 1] - x[idx]);

	return (1 - s) * y[idx] + s * y[idx + 1];

}