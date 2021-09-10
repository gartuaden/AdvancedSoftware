#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>


void program2_2()
{
	FILE* fp_r, *fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/
	int nr;
	int i = 0, j =0;
	int n;
	double h;
	double* y;
	double* x;
	double iseed;
	double xn, an, bn, an1, bn1;
	bool flag = false;

	srand(time(NULL));
	fscanf(fp_r, "%d %lf", &n, &h);

	x = (double*)malloc(sizeof(double) * n);
	y = (double*)malloc(sizeof(double) * n);

	for (i = 0; i < n; i++) {
		fscanf(fp_r, "%lf %lf", &x[i], &y[i]);
	}

	printf("난수 개수 nr을 입력하시오: ");
	scanf("%d", &nr);

	fprintf(fp_w, "%d\n", nr);


	for (i = 0; i < nr; i++) {
		iseed = (double)rand() / (RAND_MAX + 1);
	//	printf("iseed is %lf\n", iseed);
		an = 0.0;
		bn = 1.0;
		j = 0;

		while (1) {
			xn = (an + bn) / 2.0;

			if (fx(xn,iseed, x, y, n) == 0) {
				fprintf(fp_w, "%.15f\n", xn);
			//	printf("xn is %.15f\n", xn);
				break;
			}

			if (j >= Nmax || fabs(fx(xn,iseed, x, y, n)) < DELTA || (fabs(bn - an) < EPSILON)) {
				fprintf(fp_w, "%.15f\n", xn);
			//	printf("xn is %.15f\n", xn);
				break;
			}

			if (fx(an, iseed, x, y, n) * fx(xn, iseed, x, y, n) < 0) {
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

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}

double fx(double xn, double seed, double*x, double*y, int n) {

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
	sum += x[0] + ((y[1] - y[0]) / (x[1] - x[0]) * (xn - x[0]) / 2.0) * (xn - x[0]) - seed;

	return sum;
}