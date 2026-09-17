#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LEN 5

void gradient_descent(double x[static ARRAY_LEN], double y[static ARRAY_LEN], 
 							 double a[static 1], double b[static 1]) 
{
	const double learning_rate = 0.05;
	const size_t iterations = 10000;

	for (size_t i = 0; i < iterations; ++i)
	{
		double part_der_a;
		double part_der_b;
		
		for (size_t j = 0; j < ARRAY_LEN; ++j)
		{
			double error = ((*a) * x[j] + (*b)) - y[j];

			part_der_b += error;
			part_der_a += error * x[j];
		}
		
		*a -= learning_rate * (part_der_a / ARRAY_LEN);
		*b -= learning_rate * (part_der_b / ARRAY_LEN);
	}
}

int main()
{
	double a = 0.0; 
	double b = 0.0; 

	double x[ARRAY_LEN] = { 1, 2, 3, 4, 5 };
	double y[ARRAY_LEN] = { 1, 2, 3, 4, 5 };
	
	gradient_descent(x, y, &a, &b);	

	printf("a: %f b: %f\n", a, b);
	printf("%f\n", a * 1 + b);
	printf("%f\n", a * 2 + b);
	printf("%f\n", a * 3 + b);
	printf("%f\n", a * 4 + b);
	printf("%f\n", a * 5 + b);
		
	return EXIT_SUCCESS;
}
