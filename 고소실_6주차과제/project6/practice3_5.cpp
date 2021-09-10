#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_5(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/

	// jac 안씀
	fvec[0] = 10.0 * x[0] - 2.0 * x[1] * x[1] + x[1] - 2.0 * x[2] - 5.0;
	fvec[1] = 8.0 * x[1] * x[1] + 4.0 * x[2] * x[2] - 9.0;
	fvec[2] = 8.0 * x[1] * x[2] + 4.0;

	/********************************/
}

void practice3_5(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 1.0, -1.0, 1.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-5.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-5.txt");
		return;
	}

	/********************************/
	double result = 0.0;
	printf("\n************************practice 3_5 ************************\n");
	printf("\n초기값 1: x(0) = (1.0, -1.0, 1.0)\n");
	fprintf(fp_w, "\n초기값 1: x(0) = (1.0, -1.0, 1.0)\n");

	hybrd1_(fcn3_5, &n, x, fvec, &tol, &info, wa, &lwa);


	printf("%lf %lf %lf\n", x[0], x[1], x[2]);
	printf("info is %d\n", info);


	fprintf(fp_w, "x[0] = %lf		x[1] = %lf		x[2] = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "info is %d\n", info);
	result = 10.0 * x[0] - 2.0 * x[1] * x[1] + x[1] - 2.0 * x[2] - 5.0;
	printf("|f1(x)| = %lf\n", fabs(result));
	fprintf(fp_w, "|f1(x)| = %lf\n", fabs(result));
	result = 8.0 * x[1] * x[1] + 4.0 * x[2] * x[2] - 9.0;
	printf("|f2(x)| = %lf\n", fabs(result));
	fprintf(fp_w, "|f2(x)| = %lf\n", fabs(result));
	result = 8.0 * x[1] * x[2] + 4.0;
	printf("|f3(x)| = %lf\n", fabs(result));
	fprintf(fp_w, "|f3(x)| = %lf\n", fabs(result));

	printf("\n초기값 2: x(0) = (1.0, 1.0, -1.0)\n");
	fprintf(fp_w, "\n초기값 2: x(0) = (1.0, 1.0, -1.0)\n");
	x[0] = 1.0, x[1] = 1.0, x[2] = -1.0;

	hybrd1_(fcn3_5, &n, x, fvec, &tol, &info, wa, &lwa);

	printf("%lf %lf %lf\n", x[0], x[1], x[2]);
	printf("info is %d\n", info);

	fprintf(fp_w, "x[0] = %lf		x[1] = %lf		x[2] = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "info is %d\n", info);

	result = 10.0 * x[0] - 2.0 * x[1] * x[1] + x[1] - 2.0 * x[2] - 5.0;
	printf("|f1(x)| = %lf\n", fabs(result));
	fprintf(fp_w, "|f1(x)| = %lf\n", fabs(result));
	result = 8.0 * x[1] * x[1] + 4.0 * x[2] * x[2] - 9.0;
	printf("|f2(x)| = %lf\n", fabs(result));
	fprintf(fp_w, "|f2(x)| = %lf\n", fabs(result));
	result = 8.0 * x[1] * x[2] + 4.0;
	printf("|f3(x)| = %lf\n", fabs(result));
	fprintf(fp_w, "|f3(x)| = %lf\n", fabs(result));

	/********************************/

	fclose(fp_w);
}