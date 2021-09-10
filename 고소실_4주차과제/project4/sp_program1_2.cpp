#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Secant Method
**********************************************/
void sp_program1_2(FILE* fp) {
	int i = 0;
	float x0, x1;
	float xn1, xn, xnn;

	printf("======= Secant Method =======\n");
	fprintf(fp, "======= Secant Method =======\n");
	printf("초기값 x0를 입력하시오: ");
	scanf("%f", &x0);

	printf("초기값 x1를 입력하시오: ");
	scanf("%f", &x1);

	fprintf(fp, "x0 = %lf\n", x0);
	fprintf(fp, "x1 = %lf\n", x1);

	xnn = x0;
	xn = x1;

	printf("i		xn1					xn0					If(xn1)I\n");
	fprintf(fp, "i		xn1					xn0					If(xn1)I\n");

	while (1) {

		printf("%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabsf(_sp_f(xn)));
		fprintf(fp, "%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabsf(_sp_f(xn)));
		xn1 = xn - _sp_f(xn) * (xn - xnn) / (_sp_f(xn) - _sp_f(xnn));

		if ((fabsf(_sp_f(xn1)) < DELTA) || (i >= Nmax) || (fabsf(xn1 - xn) < EPSILON)) {
			i++;
			xnn = xn;
			xn = xn1;
			printf("%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabsf(_sp_f(xn)));
			fprintf(fp, "%d		%20.18e		%20.18e		%20.18e\n", i, xn, xnn, fabsf(_sp_f(xn)));
			break;
		}

		xnn = xn;
		xn = xn1;
		i++;
	}
}
