#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include <tgmath.h>

#define W 1000
#define H 1000

bool is_bound(float x, float y)
{
	double complex c = y + x * I;
	double complex z = 0.0 + 0.0 * I;
	for (int i = 0; i < 1000; ++i) {
		z = z * z + c;
		if (cabs(z) > 2.0) return false;
	}
	return true;
}

int main()
{
	printf("P3\n %d %d\n255\n", W, H);

	for (int i = 0; i < W; i++) {
		for (int j = 0; j < H; j++) {
			float x = (i * 4.0) / W - 2.0; 
			float y = (j * 4.0) / H - 2.0; 
			if (is_bound(x, y)) printf("0 0 0\n");
			else printf("255 255 255\n");
		}
	}
	
	return 0;
}
