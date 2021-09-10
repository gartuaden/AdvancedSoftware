#include "my_solver.h"

void practice3_2() {
	char readfile[256];
	char writefile[256];

	int NCOEF, DEGREE;

	double* poly;
	double* zeror, * zeroi;
	long int fail;
	int i;
	printf("\n************************practice 3_2 ************************\n");
	for (int t = 1; t <= 6; t++) {

		sprintf(readfile, "polynomial_3-2_%d.txt", t);
		sprintf(writefile, "roots_3-2_%d.txt", t);

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

		fscanf(fp_r, "%d", &DEGREE);
		fprintf(fp_w, "DEGREE = %d\n", DEGREE);
		NCOEF = DEGREE + 1;

		poly = (double*)malloc(sizeof(double) * NCOEF);
		zeror = (double*)malloc(sizeof(double) * DEGREE);
		zeroi = (double*)malloc(sizeof(double) * DEGREE);


		/********************************/
		printf("t = %d, DEGREE = %d\n", t, DEGREE);
		for (i = 0; i < NCOEF; i++) {
			fscanf(fp_r, "%lf", &poly[i]);
		}

		int degree = DEGREE;
		double f = 0.0;
		long int fail;

		rpoly_(poly, &degree, zeror, zeroi, &fail);

		if (fail) {
			printf("wrong\n");
			fprintf(fp_w, "wrong\n");
			printf("\n");
			fprintf(fp_w, "\n");
			continue;
		}
		for (i = 0; i < degree; i++) {
			printf("%10f ", zeror[i]);
			fprintf(fp_w, "zeror[%d] = %10f		", i, zeror[i]);
		}
		fprintf(fp_w, "\n");
		printf("\n");

		for (i = 0; i < degree; i++) {
			printf("%10f ", zeroi[i]);
			fprintf(fp_w, "zeroi[%d] = %10f		", i, zeroi[i]);
			if (zeroi[i] == 0) {
				int j = 0;
				for (int k = DEGREE; k > 0; k--) {
					f += poly[j] * pow(zeror[i], k);
					j++;
				}
				f += poly[j];
				printf("|f(x*)| = %10f		", fabs(f));
				fprintf(fp_w, "|f(x*)| = %10f		", fabs(f));
			}
		}
		printf("\n");
		fprintf(fp_w, "\n");
		printf("\n");
		fprintf(fp_w, "\n");

		/********************************/


		free(zeroi);
		free(zeror);
		free(poly);

		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}