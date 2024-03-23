#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in row-major order.
 */
double get(matrix *mat, int row, int col) {
    // Task 1.1 TODO
    return mat->data[row*(mat->cols)+col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val) {
    // Task 1.1 TODO
    mat->data[row*(mat->cols)+col] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    // Task 1.2 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Allocate space for the matrix data, initializing all entries to be 0. Return -2 if allocating memory failed.
    // 4. Set the number of rows and columns in the matrix struct according to the arguments provided.
    // 5. Set the `parent` field to NULL, since this matrix was not created from a slice.
    // 6. Set the `ref_cnt` field to 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0) {
        return -1;
    }
    *mat = (matrix*)malloc(sizeof(matrix));
    if (*mat == NULL) {
        free(*mat);
        return -2;
    }
    (*mat)->data = (double*)calloc(rows*cols, sizeof(double));
    if ((*mat)->data == NULL) {
        free((*mat)->data);
        free(*mat);
        return -2;
    }
    (*mat)->parent = NULL;
    (*mat)->cols = cols;
    (*mat)->rows = rows;
    (*mat)->ref_cnt = 1;
    return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent
 * matrix has no other references (including itself).
 */
void deallocate_matrix(matrix *mat) {
    // Task 1.3 TODO
    // HINTS: Follow these steps.
    // 1. If the matrix pointer `mat` is NULL, return.
    // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the `ref_cnt` field becomes 0, then free `mat` and its `data` field.
    // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then free `mat`.
    if (mat == NULL) return;
    if (mat->parent == NULL && mat->ref_cnt <= 1) {
        free(mat->data);
        free(mat);
    } else if (mat->ref_cnt > 1) {
        mat->ref_cnt -= 1;
    } else {
        mat->parent->ref_cnt -= 1;
        if (mat->parent->ref_cnt <= 0) {
            deallocate_matrix(mat->parent);
        }
        free(mat);
    }
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`
 * and the reference counter for `from` should be incremented. Lastly, do not forget to set the
 * matrix's row and column values as well.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 * NOTE: Here we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    // Task 1.4 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Set the `data` field of the new struct to be the `data` field of the `from` struct plus `offset`.
    // 4. Set the number of rows and columns in the new struct according to the arguments provided.
    // 5. Set the `parent` field of the new struct to the `from` struct pointer.
    // 6. Increment the `ref_cnt` field of the `from` struct by 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0) {
        return -1;
    }
    *mat = (matrix*)malloc(sizeof(matrix));
    if ((*mat) == NULL) {
        return -2;
    }
    (*mat)->data = from->data + offset;
    (*mat)->cols = cols;
    (*mat)->rows = rows;
    (*mat)->parent = from;
    from->ref_cnt += 1;
    return 0;
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val) {
    // Task 1.5 TODO
    int num_threads = omp_get_num_threads();
    int thread_id = omp_get_thread_num();
    int size = (mat->rows)*(mat->cols);
    double *mat_data = mat->data;
    __m256d vector = _mm256_set1_pd (val);
    #pragma omp parallel for
    for (int i=thread_id; i<(size/num_threads)*num_threads; i+=num_threads) {
        _mm256_storeu_pd(mat_data+i, vector);
        _mm256_storeu_pd(mat_data+i+4, vector);
        _mm256_storeu_pd(mat_data+i+8, vector);
        _mm256_storeu_pd(mat_data+i+12, vector);
    }
    for (int i=(size/num_threads)*num_threads+thread_id; i<size; i+=num_threads) {
        mat->data[i] = val;
    }
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {
    // Task 1.5 TODO
    double *result_data = result->data;
    double *mat_data = mat->data;

    __m256d mat_elem = _mm256_setzero_pd();
    __m256d neg_result = _mm256_setzero_pd();
    __m256d abs_result = _mm256_setzero_pd();
    __m256d all_zero = _mm256_setzero_pd();

    int size = (mat->rows)*(mat->cols);
    int num_threads = omp_get_num_threads();
    int thread_id = omp_get_thread_num();

    for (int i=thread_id; i<(size/16)*16; i+=16*num_threads) {
        mat_elem = _mm256_loadu_pd(mat_data+i);
        neg_result = _mm256_sub_pd(_mm256_set1_pd((double)0.0), mat_elem);
        abs_result = _mm256_max_pd(neg_result, mat_elem);
        _mm256_storeu_pd(result_data + i, abs_result);

        mat_elem = _mm256_loadu_pd(mat_data+i+4);
        neg_result = _mm256_sub_pd(_mm256_set1_pd((double)0.0), mat_elem);
        abs_result = _mm256_max_pd(neg_result, mat_elem);
        _mm256_storeu_pd(result_data + i + 4, abs_result);

        mat_elem = _mm256_loadu_pd(mat_data+i+8);
        neg_result = _mm256_sub_pd(_mm256_set1_pd((double)0.0), mat_elem);
        abs_result = _mm256_max_pd(neg_result, mat_elem);
        _mm256_storeu_pd(result_data + i + 8, abs_result);

        mat_elem = _mm256_loadu_pd(mat_data+i+12);
        neg_result = _mm256_sub_pd(_mm256_set1_pd((double)0.0), mat_elem);
        abs_result = _mm256_max_pd(neg_result, mat_elem);
        _mm256_storeu_pd(result_data + i + 12, abs_result);
    }
    #pragma omp parallel for
    for (int i=(size/16)*16+thread_id; i<size; i+=num_threads) {
        result->data[i] = (mat->data[i])*((mat->data[i]>0)-(mat->data[i]<0));
    }
    return 0;   
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat) {
    // Task 1.5 TODO
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.5 TODO
    int size = (mat1->rows)*(mat1->cols);
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        result->data[i] = mat1->data[i] + mat2->data[i];
    }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.5 TODO
    }

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 * You may assume `mat1`'s number of columns is equal to `mat2`'s number of rows.
 * Note that the matrix is in row-major order.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.6 TODO
    double *trans  = malloc(mat2->rows*mat2->cols*sizeof(double));
    #pragma omp parallel for
    for (int i=0; i<(mat2->rows)*(mat2->cols); i++) {
            trans[i] = mat2->data[(i%mat2->rows)*mat2->cols + i/mat2->rows];
    }
    #pragma omp parallel for
    for (int i = 0; i < (mat2->rows)*(mat2->cols); i++) {
            trans[i] = mat2->data[(i%mat2->rows)*mat2->cols + i/mat2->rows];
        }
        #pragma omp parallel for
        for (int j=0; j<(result->rows)*(result->cols); j++) {
            double sum = 0.0;
            __m256d sums = _mm256_set1_pd(0.0);
            int indexmat1 = (mat1->cols)*(j/result->cols); 
            int indextrans = (mat1->cols)*(j%result->cols);
            for (int i=0 ; i<(mat1->cols/16)*16; i+=16) { 
                sums = _mm256_fmadd_pd(_mm256_loadu_pd(mat1->data+indexmat1+i), _mm256_loadu_pd(trans+indextrans+i), sums);
                sums = _mm256_fmadd_pd(_mm256_loadu_pd(mat1->data+indexmat1+i+4), _mm256_loadu_pd(trans+indextrans+i+4), sums);
                sums = _mm256_fmadd_pd(_mm256_loadu_pd(mat1->data+indexmat1+i+8), _mm256_loadu_pd(trans+indextrans+i+8), sums);
                sums = _mm256_fmadd_pd(_mm256_loadu_pd(mat1->data+indexmat1+i+12), _mm256_loadu_pd(trans+indextrans+i+12), sums);
            }
            sum = sums[0] + sums[1] + sums[2] + sums[3];
            for (int i = (mat1->cols)/16*16; i < mat1->cols; i++) {
                sum += mat1->data[indexmat1+i] * trans[indextrans+i];
            }
            result->data[j] = sum;
        }
    free(trans);
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 * You may assume `mat` is a square matrix and `pow` is a non-negative integer.
 * Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    // Task 1.6 TODO
    int counter = 0;
    int size = (mat->rows)*(mat->cols);
    if (pow == 0) {
        for (int i = 0; i < mat->rows; i++) {
            for (int j = 0; j < mat->rows; j++) {
                if (counter == j) {
                    result->data[i*mat->rows+j] = 1.0;
                }
                else {
                    result->data[i*mat->rows+j] = 0.0;
                }
            }
            counter++;
        }
        return 0;
    } else if (pow == 1) {
        memcpy(result->data, mat->data, size*sizeof(double));
        return 0;
    }
    matrix *squared = NULL;
    allocate_matrix(&squared, mat->rows, mat->cols);
    matrix *temp = NULL;
    allocate_matrix(&temp, mat->rows, mat->cols);
    mul_matrix(squared, mat, mat);
    pow_matrix(temp, squared, pow/2);
    if (pow%2 == 1) {
        mul_matrix(result, temp, mat);
    } else {
        memcpy(result->data, temp->data, size*sizeof(double));
    }
    deallocate_matrix(squared);
    deallocate_matrix(temp);
    return 0;
}
