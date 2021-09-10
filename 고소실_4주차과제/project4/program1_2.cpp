#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Secant Method
**********************************************/
void program1_2(FILE* fp) {
	int i = 0;
	double x0, x1;
	double xn1, xn, xnn;

	printf("======= Secant Method =======\n");
	fprintf(fp, "======= Secant Method =======\n");
	printf("초기값 x0를 입력하시오: ");
	scanf("%lf", &x0);

	printf("초기값 x1를 입력하시오: ");
	scanf("%lf", &x1);

	fprintf(fp, "x0 = %lf\n", x0);
	fprintf(fp, "x1 = %lf\n", x1);
	xnn = x0;
	xn = x1;

	printf("i		xn1					xn0					If(xn1)I\n");
	fprintf(fp, "i		xn1					xn0					If(xn1)I\n");


	while (1) {

		printf("%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabs(_f(xn)));
		fprintf(fp, "%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabs(_f(xn)));
		xn1 = xn - _f(xn) * (xn - xnn) / (_f(xn) - _f(xnn));

		if ((fabs(_f(xn1)) < DELTA) || (i >= Nmax) || (fabs(xn1 - xn) < EPSILON)) {
			i++;
			xnn = xn;
			xn = xn1;
			printf("%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabs(_f(xn)));
			fprintf(fp, "%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabs(_f(xn)));
			break;
		}

		xnn = xn;
		xn = xn1;
		i++;
	}
}
