#include "my_solver.h"

void program2_1()
{
	FILE* fp_r, *fp_w;
	__int64 start, freq, end;
	float resultTime = 0;

	fp_r = fopen("sampling_table.txt", "r");
	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf_table.txt", "w");

	/***************************************************/

	int n1, n;
	double h,new_h;
	double* x;
	double x_min = 0;
	double x_max = 0;
	double sum = 0, sum_y = 0, check_sum=0;
	double* y;
	double* new_x;
	double* new_y;
	int i,j;
	int min=0, max=0;

	fscanf(fp_r, "%d %lf", &n1, &h);
	// printf("n1 is %lf , h is %lf\n", n1, h);

	x = (double*)malloc(sizeof(double) * n1);
	y = (double*)malloc(sizeof(double) * n1);
	new_x = (double*)malloc(sizeof(double) * n1);
	new_y = (double*)malloc(sizeof(double) * n1);

	for (i = 0; i < n1; i++) {
		fscanf(fp_r, "%lf %lf", &x[i], &y[i]);
	}

	x_min = x[0];
	x_max = x[n1-1];

	// normalization of x
	for (i = 0; i < n1; i++) {
		new_x[i] = (double)(x[i] - x_min) / (x_max - x_min);
	}

	// 구분 구적법

	for (i = 0; i < n1; i++) {
		if (i + 1 == n1) break;
		sum += (new_x[1]-new_x[0]) / 2*(y[i+1]+y[i]);
	}

	for (i = 0; i < n1; i++) {
		new_y[i] = y[i] / sum;
	}

	new_h = 1/(double)(n1-1);

	fprintf(fp_w, "%d %lf\n", n1, new_h);

	for (i = 0; i < n1; i++) {
		fprintf(fp_w, "%lf %lf\n", (double)new_x[i],(double)new_y[i]);
	}

	sum = 0;
	for (i = 0; i < n1; i++) {
		if (i + 1 == n1) break;
		sum += (new_x[1] - new_x[0]) / 2 * (new_y[i + 1] + new_y[i]);
	}

	printf("*** integrating the pdf from 0.0 to 1.0 = %.6f\n", sum);

	sum = 0;

	for (i = 0; i < n1; i++) {
		if (new_x[i] >= 0.25) {
			break;
		}
		max = i;
	}


	// 0~max 사다리꼴
	for (int i = 0; i < max + 1; i++) {
		if (i + 1 == max + 1) break;
		sum += (new_x[1] - new_x[0]) / 2 * (new_y[i + 1] + new_y[i]);
	}

	// 나머지 구간
	// 뒷 부분
	sum += (new_y[max] + (new_y[max + 1] - new_y[max]) / (new_x[max + 1] - new_x[max]) * (0.25 - new_x[max] / 2)) * (0.25 - new_x[max]);

	printf("*** integrating the pdf from 0.0 to 0.25 = %.6f\n", sum);

	sum = 0;

	for (i = 0; i < n1; i++) {
		if (new_x[i] >= 0.25) {
			min = i;
			break;
		}
	}

	for (i = 0; i < n1; i++) {
		if (new_x[i] >= 0.5) {
			break;
		}
		max = i;
	}

	// min~max 사다리꼴
	for (int i = min; i < max+1; i++) {
		if (i + 1 == max + 1) break;
		sum += (new_x[1] - new_x[0]) / 2 * (new_y[i + 1] + new_y[i]);
	}

	// 나머지 구간
	// 앞 부분
	sum += (new_y[min] + (new_y[min + 1] - new_y[min]) / (new_x[min + 1] - new_x[min])*(0.25-new_x[min]/2)) * (0.25 - new_x[min]);
	// 뒷 부분
	sum += (new_y[max] + (new_y[max + 1] - new_y[max]) / (new_x[max + 1] - new_x[max]) * (0.5 - new_x[max] / 2)) * (0.5 - new_x[max]);

	printf("*** integrating the pdf from 0.25 to 0.5 = %.6f\n", sum);


	sum = 0;

	for (i = 0; i < n1; i++) {
		if (new_x[i] >= 0.5) {
			min = i;
			break;
		}
	}

	for (i = 0; i < n1; i++) {
		if (new_x[i] >= 0.75) {
			break;
		}
		max = i;
	}

	// min~max 사다리꼴
	for (int i = min; i < max+1; i++) {
		if (i + 1 == max + 1) break;
		sum += (new_x[1] - new_x[0]) / 2 * (new_y[i + 1] + new_y[i]);
	}

	// 나머지 구간
	// 앞 부분
	sum += (new_y[min] + (new_y[min + 1] - new_y[min]) / (new_x[min + 1] - new_x[min]) * (0.5 - new_x[min] / 2)) * (0.5 - new_x[min]);
	// 뒷 부분
	sum += (new_y[max] + (new_y[max + 1] - new_y[max]) / (new_x[max + 1] - new_x[max]) * (0.75 - new_x[max] / 2)) * (0.75 - new_x[max]);

	printf("*** integrating the pdf from 0.5 to 0.75 = %.6f\n", sum);


	sum = 0;

	for (i = 0; i < n1; i++) {
		if (new_x[i] >= 0.75) {
			min = i;
			break;
		}
	}

	// min~1 사다리꼴
	for (int i = min; i < n1; i++) {
		if (i + 1 == n1) break;
		sum += (new_x[1] - new_x[0]) / 2 * (new_y[i + 1] + new_y[i]);
	}

	// 나머지 구간
	// 앞 부분
	sum += (new_y[min] + (new_y[min + 1] - new_y[min]) / (new_x[min + 1] - new_x[min]) * (0.75 - new_x[min] / 2)) * (0.75 - new_x[min]);

	printf("*** integrating the pdf from 0.75 to 1.0 = %.6f\n", sum);


	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}
