#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Newton-Rapson Method
**********************************************/
void sp_program1_1(FILE* fp) {
	int i = 0;
	float x0;
	float xn1, xn;

	printf("======= Newton-Rapson Method =======\n");
	fprintf(fp, "======= Newton-Rapson Method =======\n");
	printf("초기값을 입력하시오: ");
	scanf("%f", &x0);
	xn = x0;

	fprintf(fp, "x0 = %lf\n", x0);

	printf("i		xn1					If(xn1)I\n");
	fprintf(fp, "i		xn1					If(xn1)I\n");

	while (1) {
		printf("%d		%20.18e		%20.18e\n", i, xn, fabsf(_sp_f(xn)));
		fprintf(fp, "%d		%20.18e		%20.18e\n", i, xn, fabsf(_sp_f(xn)));
		xn1 = xn - (_sp_f(xn) / _sp_fp(xn));

		if ((fabsf(_sp_f(xn1)) < DELTA) || (i >= Nmax) || (fabsf(xn1 - xn) < EPSILON)) {
			i++;
			xn = xn1;
			printf("%d		%20.18e		%20.18e\n", i, xn, fabsf(_sp_f(xn)));
			fprintf(fp, "%d		%20.18e		%20.18e\n", i, xn, fabsf(_sp_f(xn)));
			break;
		}

		xn = xn1;
		i++;
	}
}
