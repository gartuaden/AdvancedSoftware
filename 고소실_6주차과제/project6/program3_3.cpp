#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void f3_3(int* n, double* x, double* fvec, int* iflag)
{

	fvec[0] = (sin(x[0] * x[1] + M_PI / 6.0) + sqrt(x[0] * x[0] * x[1] * x[1] + 1.0)) / cos(x[0] - x[1]) + 2.8;
	fvec[1] = (x[0] * exp(x[0] * x[1] + M_PI / 6.0) - sin(x[0] - x[1])) / sqrt(x[0] * x[0] * x[1] * x[1] + 1.0) - 1.66;

}

void program3_3(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.0, 0.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;
	double result = 0.0;

	FILE* fp_w = fopen("roots_found_3-3.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3-3.txt");
		return;
	}

	x[0] = 20.0, x[1] = 0.0;
	printf("초기값 x1 = %lf, x2 =  %lf\n", x[0], x[1]);
	fprintf(fp_w, "초기값 x1 = %lf, x2 = %lf\n", x[0], x[1]);

	hybrd1_(f3_3, &n, x, fvec, &tol, &info, wa, &lwa);

	printf("x[0] = %lf, x[1] =  %lf\n", x[0], x[1]);
	printf("info is %d\n", info);

	fprintf(fp_w, "x[0] = %lf, x[1] =  %lf\n", x[0], x[1]);
	fprintf(fp_w, "info is %d\n", info);

	result = (sin(x[0] * x[1] + M_PI / 6.0) + sqrt(x[0] * x[0] * x[1] * x[1] + 1.0)) / (cos(x[0] - x[1])) + 2.8;
	fprintf(fp_w, "|f1(x[0], x[1])| = %lf\n", fabs(result));
	printf("|f1(x[0], x[1])| = %lf\n", fabs(result));

	result = (x[0] * exp(x[0] * x[1] + M_PI / 6.0) - sin(x[0] - x[1])) / sqrt(x[0] * x[0] * x[1] * x[1] + 1.0) - 1.66;
	fprintf(fp_w, "|f2(x[0], x[1])| = %lf\n", fabs(result));
	printf("|f2(x[0], x[1])| = %lf\n", fabs(result));

}