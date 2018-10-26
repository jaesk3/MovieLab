#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

int SumN(int N, ...);

int main() {

	int sum1 = SumN(5, 1, 2, 3, 4, 5);
	printf("sum1 = %d\n", sum1);
	int array[5] = {5, 10, 15, 20, 25};
	int *pt;

	char s[] = "Hppe!Mvdl!boe!Ibqqz!Ipmjebzt";
	char *p;
	p = &s[0];
	
	while (*p) { 	
		printf("%c", *p - 1);
		p++;
	}
	printf("\n");


	return 0;
}

int SumN(int N, ...)
{
	va_list ap;
	int i, a, s = 0;
	va_start(ap, N);

	for (i = 0; i < N; i++) {
		a = va_arg(ap, int);
		printf("i = %d, a = %d\n", i, a);
		s += a;
	}
	va_end(ap);

	return s;
}