#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Bisection Method
**********************************************/
void program1_3(FILE* fp) {
	int i = 0;
	double a0, b0;
	double an, bn, xn, an1, bn1;


	printf("\n======= Bisection Method =======\n");
	fprintf(fp, "\n======= Bisection Method =======\n");
	printf("초기 구간의 경계값 a0을 입력하시오: ");
	scanf("%lf", &a0);
	printf("초기 구간의 경계값 b0을 입력하시오: ");
	scanf("%lf", &b0);

	fprintf(fp, "a0 = %lf\n", a0);
	fprintf(fp, "b0 = %lf\n", b0);

	an = a0;
	bn = b0;


	printf("i		an				bn					xn				lf(xn)l\n");
	fprintf(fp, "i		an				bn					xn				lf(xn)l\n");


	while (1) {

		xn = (an + bn) / 2.0;

		if (_f(xn) == 0) { // it is exact root
			printf("%d		%12.10e		%12.10e			%12.10e		%12.10e\n", i, an, bn, xn, fabs(_f(xn)));
			fprintf(fp, "%d		%12.10e		%12.10e			%12.10e		%12.10e\n", i, an, bn, xn, fabs(_f(xn)));
			break;
		}


		if (i >= Nmax || fabs(_f(xn)) < DELTA || (fabs(bn - an) < EPSILON)) {

			i++;

			printf("%d		%12.10e		%12.10e			%12.10e		%12.10e\n", i, an, bn, xn, fabs(_f(xn)));
			fprintf(fp, "%d		%12.10e		%12.10e			%12.10e		%12.10e\n", i, an, bn, xn, fabs(_f(xn)));
			break;
		}


		if (_f(an) * _f(xn) < 0) { // if f(a) and f(c) have different signs, then b = x
			bn1 = xn;
			an1 = an;
		}
		else { // otherwise, a = x
			an1 = xn;
			bn1 = bn;
		}

		printf("%d		%12.10e		%12.10e			%12.10e		%12.10e\n", i, an, bn, xn, fabs(_f(xn)));
		fprintf(fp, "%d		%12.10e		%12.10e			%12.10e		%12.10e\n", i, an, bn, xn, fabs(_f(xn)));


		an = an1;
		bn = bn1;
		i++;
	}


}