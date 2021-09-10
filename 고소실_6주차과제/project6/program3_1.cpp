#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double p[4][5] = {};
double C = 0.0, b = 0.0;


void f3_1_b(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/

	fvec[0] = (x[0] - p[0][0]) * (x[0] - p[0][0]) + (x[1] - p[0][1]) * (x[1] - p[0][1]) + (x[2] - p[0][2]) * (x[2] - p[0][2]) - (C * (p[0][4] + x[3] - p[0][3])) * (C * (p[0][4] + x[3] - p[0][3]));
	fvec[1] = (x[0] - p[1][0]) * (x[0] - p[1][0]) + (x[1] - p[1][1]) * (x[1] - p[1][1]) + (x[2] - p[1][2]) * (x[2] - p[1][2]) - (C * (p[1][4] + x[3] - p[1][3])) * (C * (p[1][4] + x[3] - p[1][3]));
	fvec[2] = (x[0] - p[2][0]) * (x[0] - p[2][0]) + (x[1] - p[2][1]) * (x[1] - p[2][1]) + (x[2] - p[2][2]) * (x[2] - p[2][2]) - (C * (p[2][4] + x[3] - p[2][3])) * (C * (p[2][4] + x[3] - p[2][3]));
	fvec[3] = (x[0] - p[3][0]) * (x[0] - p[3][0]) + (x[1] - p[3][1]) * (x[1] - p[3][1]) + (x[2] - p[3][2]) * (x[2] - p[3][2]) - (C * (p[3][4] + x[3] - p[3][3])) * (C * (p[3][4] + x[3] - p[3][3]));

	/********************************/
}

void f3_1_a(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/

		fvec[0] = (x[0] - p[0][0]) * (x[0] - p[0][0]) + (x[1] - p[0][1]) * (x[1] - p[0][1]) + (x[2] - p[0][2]) * (x[2] - p[0][2]) - (C * (p[0][4] + x[3] - p[0][3])) * (C * (p[0][4] + x[3] - p[0][3]));
		fvec[1] = (x[0] - p[1][0]) * (x[0] - p[1][0]) + (x[1] - p[1][1]) * (x[1] - p[1][1]) + (x[2] - p[1][2]) * (x[2] - p[1][2]) - (C * (p[1][4] + x[3] - p[1][3])) * (C * (p[1][4] + x[3] - p[1][3]));
		fvec[2] = (x[0] - p[2][0]) * (x[0] - p[2][0]) + (x[1] - p[2][1]) * (x[1] - p[2][1]) + (x[2] - p[2][2]) * (x[2] - p[2][2]) - (C * (p[2][4] + x[3] - p[2][3])) * (C * (p[2][4] + x[3] - p[2][3]));
		fvec[3] = (x[0] - p[3][0]) * (x[0] - p[3][0]) + (x[1] - p[3][1]) * (x[1] - p[3][1]) + (x[2] - p[3][2]) * (x[2] - p[3][2]) - (C * (p[3][4] + x[3] - p[3][3])) * (C * (p[3][4] + x[3] - p[3][3]));
		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/

		fjac[0] = 2.0 * x[0] - 2.0 * p[0][0];			fjac[4] = 2.0 * x[1] - 2.0 * p[0][1];			fjac[8] = 2.0 * x[2] - 2.0 * p[0][2]; 		fjac[12] = -2.0 * C * C * (x[3] + p[0][4] - p[0][3]);
		fjac[1] = 2.0 * x[0] - 2.0 * p[1][0];			fjac[5] = 2.0 * x[1] - 2.0 * p[1][1];			fjac[9] = 2.0 * x[2] - 2.0 * p[1][2]; 		fjac[13] = -2.0 * C * C * (x[3] + p[1][4] - p[1][3]);
		fjac[2] = 2.0 * x[0] - 2.0 * p[2][0];			fjac[6] = 2.0 * x[1] - 2.0 * p[2][1];			fjac[10] = 2.0 * x[2] - 2.0 * p[2][2]; 		fjac[14] = -2.0 * C * C * (x[3] + p[2][4] - p[2][3]);
		fjac[3] = 2.0 * x[0] - 2.0 * p[3][0];			fjac[7] = 2.0 * x[1] - 2.0 * p[3][1];			fjac[11] = 2.0 * x[2] - 2.0 * p[3][2]; 		fjac[15] = -2.0 * C * C * (x[3] + p[3][4] - p[3][3]);
		/********************************/
	}
}

void program3_1(void) {
	char readfile[256];
	char writefile[256];


	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.0, 0.0, 0.0, 0.0 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;
//	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
//	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;
	double result = 0.0;


	for (int k = 0; k <= 2; k++) {
		sprintf(readfile, "GPS_signal_%d.txt", k);
		sprintf(writefile, "GPS_position_3-1_%d.txt", k);
	//	sprintf(writefile, "GPS_position_3-2_%d.txt", k);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}
		
		fscanf(fp_r, "%lf %lf", &C, &b);
		printf("초기값 x1, x2, x3을 입력하시오: ");
		scanf("%lf %lf %lf", &x[0], &x[1], &x[2]);
		x[3] = b;
		
		fprintf(fp_w, "초기값 x1 = %lf, x2 =  %lf, x3 =  %lf, x4 = %lf\n", x[0], x[1], x[2], x[3]);
		for (int i = 0; i < 4; i++) {
			fscanf(fp_r, "%lf %lf %lf %lf %lf", &p[i][0], &p[i][1], &p[i][2], &p[i][3], &p[i][4]);
		}

		hybrj1_(f3_1_a, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);


	//	hybrd1_(f3_1_b, &n, x, fvec, &tol, &info, wa, &lwa);
		
		printf("x[0] = %lf, x[1] =  %lf, x[2] =  %lf, x[3] = %lf\n", x[0], x[1], x[2], x[3]);
		printf("info is %d\n", info);

		fprintf(fp_w, "x[0] = %lf, x[1] =  %lf, x[2] =  %lf, x[3] = %lf\n", x[0], x[1], x[2], x[3]);
		fprintf(fp_w, "info is %d\n", info);

		result = (x[0] - p[0][0]) * (x[0] - p[0][0]) + (x[1] - p[0][1]) * (x[1] - p[0][1]) + (x[2] - p[0][2]) * (x[2] - p[0][2]) - (C * (p[0][4] + x[3] - p[0][3])) * (C * (p[0][4] + x[3] - p[0][3]));
		fprintf(fp_w, "|f1(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));
		printf("|f1(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));

		result = (x[0] - p[1][0]) * (x[0] - p[1][0]) + (x[1] - p[1][1]) * (x[1] - p[1][1]) + (x[2] - p[1][2]) * (x[2] - p[1][2]) - (C * (p[1][4] + x[3] - p[1][3])) * (C * (p[1][4] + x[3] - p[1][3]));
		fprintf(fp_w, "|f2(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));
		printf("|f2(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));


		result = (x[0] - p[2][0]) * (x[0] - p[2][0]) + (x[1] - p[2][1]) * (x[1] - p[2][1]) + (x[2] - p[2][2]) * (x[2] - p[2][2]) - (C * (p[2][4] + x[3] - p[2][3])) * (C * (p[2][4] + x[3] - p[2][3]));
		fprintf(fp_w, "|f3(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));
		printf("|f3(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));

		result = (x[0] - p[3][0]) * (x[0] - p[3][0]) + (x[1] - p[3][1]) * (x[1] - p[3][1]) + (x[2] - p[3][2]) * (x[2] - p[3][2]) - (C * (p[3][4] + x[3] - p[3][3])) * (C * (p[3][4] + x[3] - p[3][3]));
		fprintf(fp_w, "|f4(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));
		printf("|f4(x[0], x[1], x[2], x[3])| = %lf\n", fabs(result));
		
		C = 0.0;
		b = 0.0;
	}


}