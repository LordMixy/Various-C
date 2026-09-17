#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>
#include <omp.h>

#define SIZE 1000000
#define PREFETCH_DISTANCE 32

int main() {
    float *a = (float *)_mm_malloc(SIZE * sizeof(float), 32);
    float *b = (float *)_mm_malloc(SIZE * sizeof(float), 32);
    float *c = (float *)_mm_malloc(SIZE * sizeof(float), 32);

    for (int i = 0; i < SIZE; i++) {
        a[i] = (float)rand() / RAND_MAX;
        b[i] = (float)rand() / RAND_MAX;
    }

    clock_t start = clock();

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i += 32) {
        _mm_prefetch((char*)&a[i + PREFETCH_DISTANCE], _MM_HINT_T0);
        _mm_prefetch((char*)&b[i + PREFETCH_DISTANCE], _MM_HINT_T0);

        __m256 vec_a1 = _mm256_load_ps(&a[i]);
        __m256 vec_b1 = _mm256_load_ps(&b[i]);
        __m256 vec_c1 = _mm256_fmadd_ps(vec_a1, vec_b1, _mm256_sub_ps(vec_a1, vec_b1));
        _mm256_store_ps(&c[i], vec_c1);

        __m256 vec_a2 = _mm256_load_ps(&a[i + 8]);
        __m256 vec_b2 = _mm256_load_ps(&b[i + 8]);
        __m256 vec_c2 = _mm256_fmadd_ps(vec_a2, vec_b2, _mm256_sub_ps(vec_a2, vec_b2));
        _mm256_store_ps(&c[i + 8], vec_c2);

        __m256 vec_a3 = _mm256_load_ps(&a[i + 16]);
        __m256 vec_b3 = _mm256_load_ps(&b[i + 16]);
        __m256 vec_c3 = _mm256_fmadd_ps(vec_a3, vec_b3, _mm256_sub_ps(vec_a3, vec_b3));
        _mm256_store_ps(&c[i + 16], vec_c3);

        __m256 vec_a4 = _mm256_load_ps(&a[i + 24]);
        __m256 vec_b4 = _mm256_load_ps(&b[i + 24]);
        __m256 vec_c4 = _mm256_fmadd_ps(vec_a4, vec_b4, _mm256_sub_ps(vec_a4, vec_b4));
        _mm256_store_ps(&c[i + 24], vec_c4);
    }

    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    long long total_flops = 3 * SIZE;
    double flops = total_flops / time_spent;

    printf("Tempo trascorso: %f secondi\n", time_spent);
    printf("FLOPS: %e\n", flops);

    _mm_free(a);
    _mm_free(b);
    _mm_free(c);

    return 0;
}
