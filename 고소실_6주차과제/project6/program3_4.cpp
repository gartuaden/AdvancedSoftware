#include "my_solver.h"


#define index_2d(row, col, dim) row*dim+col


void program3_4(void) {

	char readfile[256];
	char writefile[256];
	sprintf(readfile, "linear_system_3-4.txt");
	sprintf(writefile, "solution_3-4.txt");

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

	int ia, n, *l;
	float* b, *a, * x, *s;


	fscanf(fp_r, "%d", &n);

	fprintf(fp_w, "%d\n", n);
	ia = n;
	l = (int*)malloc(sizeof(int) * n);
	a = (float*)malloc(sizeof(float) * n*n);

	b = (float*)malloc(sizeof(float) * n);
	x = (float*)malloc(sizeof(float) * n);
	s = (float*)malloc(sizeof(float) * n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			fscanf(fp_r, "%f", &a[index_2d(i, j, n)]);
			//printf("%f\n", a[index_2d(i, j, n)]);
		}
	}
	for (int i = 0; i < n; i++) {
		fscanf(fp_r, "%f", &b[i]);
	}
	gespp_(&n, a, &ia, l, s);
	solve_(&n, a, &ia, l, b, x);

	printf("***** Solution *****\n");

	float test = 0.0;

	for (int i = 0; i < n; i++) {
		printf("x[%d] = %.6f\n", i, x[i]);
		fprintf(fp_w, "%.6f\n", x[i]);
	}

	
	float sum = 0.0, pow_sum = 0.0, pow_sum2 = 0.0,  result_e = 0.0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			sum += a[index_2d(i, j, n)] * x[j];
		}
		sum -= b[i];
		pow_sum += sum * sum;
		sum = 0.0;
	}
	for (int i = 0; i < n; i++) {
		pow_sum2 += b[i] * b[i];
	}

	result_e = sqrt(pow_sum) / sqrt(pow_sum2);

	printf("%.6f\n", result_e);
	fprintf(fp_w, "%.6f\n", result_e);

}