#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

static int32_t K[] = 
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

uint8_t* to_bits_padded(char* s, size_t* len)
{
	// Sia S una stringa di n caratteri.
	// (n * 8) bit sono necessari per la rappresentazione.
	// NumBits(S) = Len(S) * 8.
	uint64_t num_bits = strlen(s) * 8;

	// todo...
	int32_t remainder = (num_bits) % 512;
	int32_t padding = remainder <= 448 ? (448 - remainder) : (512 - remainder + 448);

	// Il messaggio risultante e' di lunghezza: NumBits(S) + Padding(S) + 64
	// Dove i 64 bit finali sono NumBits(S) in rappresentazione binaria 
	*len = num_bits + padding + 64;	
	uint8_t* bits = malloc(sizeof(uint8_t) * (*len));

	// Per ogni carattere C_i della stringa S,
	// Siano B(C_i) = (B_i0, ..., B_i8) i bit 
	// del carattere C_i. 
	// Aggiungere B(C_i) all'array che contiene
	// i bit. 
	for (size_t i = 0; i < strlen(s); ++i) {
		uint8_t c = (uint8_t) s[i];
		for (int8_t j = 7; j >= 0; --j) {
			// md5 usa la rappresentazione little-endian,
			// verra' aggiunto prima il bit in posizione 7,
			// poi 6, ..., 0. 
			// (c & (1 << j)) >> j e' il j-esimo bit di c.
			// equivalente a: (c >> j) & 1.
			// (7 - j) + (8 * i) nel i-esima iterazione 
			// e' nel range [(8 * i), (8 * (i + 1)) - 1],
			// quindi: [0, 7], [8, 15], ...
			bits[(7 - j) + (8 * i)] = (c & (1 << j)) >> j;
		}
	}

	// Aggiungere 1 
	bits[num_bits] = 1;

	// Il padding del messaggio sono tutti 0 bit, 
	// dunque: MSG[NumBits(S) .. (NumBits[S] + Padding)] = 0,
	for (size_t i = num_bits + 1; i < (*len - 64); ++i) {
		bits[i] = 0;
	}

	// Aggiungere la lunghezza
	for (int8_t i = 0; i < 64; ++i) {
		bits[num_bits + padding + 63 - i] = (num_bits >> i) & 1;
	}
	
	return bits;
}

int main() 
{
	char* s = "a";

	size_t bits_len;
	uint8_t* bits = to_bits_padded(s, &bits_len);

	uint8_t chunk[16][32];
	for (size_t j = 0; j < 16; ++j) {
		for (size_t z = 0; z < 32; ++z) {
			chunk[j][z] = bits[(32 * j) + z];
		}
	}		

	int a0 = A;
	int b0 = B;
	int c0 = C;
	int d0 = D;

	for (size_t i = 0; i < 63; ++i) {
		int f, g;
		if (i <= 15) {
			f = (b0 & c0) | (~b0 & d0);
			g = i;
		} else if (i <= 31 && i >= 16) {
			f = (d0 & b0) | ((~b0) & d0);
			g = (5 * i + 1) % 16;
		} else if (i <= 47 && i >= 32) {
			f = b0 ^ c0 ^ d0;
			g = (3 * i + 1) % 16;
		} else if (i <= 63 && i >= 48) {
			f = c0 ^ (b0 | (~d0));
			g = (7 * i + 1) % 16;
		} 

		f += a0 + K[i] + M[g]; 
	}

	
	free(bits);
	return 0;
}
