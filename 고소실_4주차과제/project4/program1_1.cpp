#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Newton-Raphson Method
**********************************************/
void program1_1(FILE* fp) {
	int i = 0;
	double x0;
	double xn1, xn;

	printf("======= Newton-Raphson Method =======\n");
	fprintf(fp, "======= Newton-Raphson Method =======\n");
	printf("초기값을 입력하시오: ");
	scanf("%lf", &x0);
	xn = x0;

	fprintf(fp, "x0 = %lf\n", x0);

	printf("i		xn1					If(xn1)I\n");
	fprintf(fp, "i		xn1					If(xn1)I\n");

	while (1) {

		printf("%d		%20.18e		%20.18e\n", i, xn, fabs(_f(xn)));
		fprintf(fp, "%d		%20.18e		%20.18e\n", i, xn, fabs(_f(xn)));
		xn1 = xn - (_f(xn) / _fp(xn));

		if ((fabs(_f(xn1)) < DELTA) || (i >= Nmax) || (fabs(xn1 - xn) < EPSILON)) {
			i++;
			xn = xn1;
			printf("%d		%20.18e		%20.18e\n", i, xn, fabs(_f(xn)));
			fprintf(fp, "%d		%20.18e		%20.18e\n", i, xn, fabs(_f(xn)));
			break;
		}

		xn = xn1;
		i++;
	}

}
