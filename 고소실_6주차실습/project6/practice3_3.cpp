#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_3(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/

		fvec[0] = exp(2 * x[0]) - x[1] + 4.0;
		fvec[1] =x[1] - x[2] * x[2] - 1.0;
		fvec[2] = x[2] - sin(x[0]);

		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/


		fjac[0] = 2.0 * exp(2 * x[0]);			fjac[3] = -1.0;			fjac[6] = 0.0;
		fjac[1] = 0.0;							fjac[4] = 1.0;			fjac[7] = - 2.0 * x[2];
		fjac[2] = -cos(x[0]);					fjac[5] = 0.0;			fjac[8] = 1.0;

		/********************************/
	}
}

void practice3_3(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.0, 0.0, 0.0 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-3.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-3.txt");
		return;
	}

	/********************************/
	printf("\n************************practice 3_3 ************************\n");
	printf("초기값 1: x(0) = (0, 0, 0)\n");
	fprintf(fp_w, "초기값 1: x(0) = (0, 0, 0)\n");
	hybrj1_(fcn3_3, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

	printf("%lf %lf %lf\n", x[0], x[1], x[2]);
	printf("info is %d\n", info);

	fprintf(fp_w, "x[0] = %lf		x[1] = %lf		x[2] = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "info is %d\n", info);


	printf("\n초기값 2: x(0) = (1.55, 1.39, 1.10)\n");
	fprintf(fp_w, "\n초기값 2: x(0) = (1.55, 1.39, 1.10)\n");

	x[0] = 1.55, x[1] = 1.39, x[2] = 1.10;

	hybrj1_(fcn3_3, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

	printf("%lf %lf %lf\n", x[0], x[1], x[2]);
	printf("info is %d\n", info);

	fprintf(fp_w, "x[0] = %lf		x[1] = %lf		x[2] = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "info is %d\n", info);

	/********************************/

	fclose(fp_w);
}
