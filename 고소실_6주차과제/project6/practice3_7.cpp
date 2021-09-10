#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_7(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/

	fvec[0] = 2.0 * x[0] * x[0] * x[0] - 12.0 * x[0] - x[1] - 1.0;
	fvec[1] = 3.0 * x[1] * x[1] - 6.0 * x[1] - x[0] - 3.0;

	/********************************/
}

void practice3_7(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { -4.0, 5.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-7.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-7.txt");
		return;
	}

	/********************************/
	double result1 = 0.0, result2 = 0.0;
	printf("\n************************practice 3_7 ************************\n");
	for (int i = -4; i <= 4; i++) {
		for (int j = 5; j >= -5; j--) {
			x[0] = i;
			x[1] = j;
			hybrd1_(fcn3_7, &n, x, fvec, &tol, &info, wa, &lwa);
			if (info == 1) {
				result1 = 2.0 * x[0] * x[0] * x[0] - 12.0 * x[0] - x[1] - 1.0;
				result2 = 3.0 * x[1] * x[1] - 6.0 * x[1] - x[0] - 3.0;
				printf("|f1(x)| = %lf, |f2(x)| = %lf,		x[0], x[1] = %lf, %lf\n", fabs(result1), fabs(result2), x[0], x[1]);
				fprintf(fp_w, "|f1(x)| = %lf, |f2(x)| = %lf,		x[0], x[1] = %lf, %lf\n", fabs(result1), fabs(result2), x[0], x[1]);
			}
		}
	}
	
	/********************************/

	fclose(fp_w);
}