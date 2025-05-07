#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef MAX_DIM
# define MAX_DIM 4000
#endif // MAX_DIM

#ifndef DEF_DIM
# define DEF_DIM 1000
#endif // DEF_DIM

static float **matrix1 = NULL;
static float **matrix2 = NULL;
static float **result = NULL;
static int dim = DEF_DIM;

float **alloc_matrix(void);
void free_matrix(float **);
void init(float **, float **, float **);
void op_mat(float **);
void mult(void);

int main(int argc, char *argv[]) {
    int opt = -1;

    while ((opt = getopt(argc, argv, "t:d:h")) != -1) {
        switch (opt) {
            case 't':
                    //
                break;
            case 'd':
                dim = atoi(optarg);
                if (dim < DEF_DIM) dim = DEF_DIM;
                if (dim > MAX_DIM) dim = MAX_DIM;
                break;
            case 'h':
                printf("%s: -t # -d #\n", argv[0]);
                printf("\t-t #: number of threads\n");
                printf("\t-d #: size of matrix\n");
                exit(0);
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    matrix1 = alloc_matrix();
    matrix2 = alloc_matrix();
    result = alloc_matrix();

    init(matrix1, matrix2, result);
    mult();
    op_mat(result);

    free_matrix(matrix1);
    free_matrix(matrix2);
    free_matrix(result);

    matrix1 = matrix2 = result = NULL;

    return EXIT_SUCCESS;
}

float **alloc_matrix(void) {
    float **mat = malloc(dim * sizeof(float *));
    for (int i = 0; i < dim; i++) {
        mat[i] = malloc(dim * sizeof(float));
    }
    return mat;
}

void free_matrix(float **mat) {
    for (int i = 0; i < dim; i++) {
        free(mat[i]);
    }
    free(mat);
}

void init(float **mat1, float **mat2, float **res) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            mat1[i][j] = (i + j) * 2.0;
            mat2[i][j] = (i + j) * 3.0;
            res[i][j] = 0.0;
        }
    }
}

void op_mat(float **mat1) {
    FILE *op = NULL;
    const char *FILE_NAME = "mm1.txt";

    op = fopen(FILE_NAME, "w");
    if (op == NULL) {
        perror("could not open file: mm1.txt");
        exit(17);
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fprintf(op, "%8.2f ", mat1[i][j]);
        }
        fprintf(op, "\n");
    }

    fclose(op);
}

void mult(void) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

