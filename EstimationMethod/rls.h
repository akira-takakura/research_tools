/*
Reast Squares Method Algorithm
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.1415

typedef struct {
    double lambda;
    double gamma;
    double M;
    double D;
    double K;
    double C;
    double RSE;
    double error_rate;
    double den;     // RLS‚ÌXV®‚Ì•ª•ê
    double epsilon; // RLS‚É‚¨‚¯‚é’€ŸŒë·
    double mu;
    double gzi;
    double I;       // RLSd‚İ, –³ŒÀ‘å‚Ålambda = 1.0
} ReastsquareMethod;

/* Matrix calculation */
// definition
#define dim 3
// mm
#define row1    dim
#define column1 1
#define row2    1
#define column2 dim

// mm
#define mm_row1    dim
#define mm_column1 dim
#define mm_row2    dim
#define mm_column2 dim

// ip
#define ip_row1    1
#define ip_column1 dim
#define ip_row2    dim
#define ip_column2 1

// mv
#define mv_row1    dim
#define mv_column1 dim
#define mv_row2    dim
#define mv_column2 1

/* xx' */
void InnerProduct_inv(double m1[][column1], double m2[][column2], double result[][column2]) {
    if (column1 != row2) { printf("[Caution!] Couldn't define the Product\n"); }

    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < column2; j++) {
            for (int k = 0; k < row2; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/* A*A */
void MatrixMatrix(double m1[][mm_column1], double m2[][mm_column2], double result[][mm_column2]) {
    if (mm_column1 != mm_row2) { printf("[Caution!] Couldn't define the Product\n"); }
    for (int i = 0; i < mm_row1; i++) {
        for (int j = 0; j < mm_column2; j++) {
            for (int k = 0; k < mm_row2; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/* x'x */
void InnerProduct(double m1[][ip_column1], double m2[][ip_column2], double* return_result) {
    if (ip_column1 != ip_row2) { printf("[Caution!] Couldn't define the Product\n"); }
    double result[ip_row1][ip_column2];

    for (int i = 0; i < ip_row1; i++) {
        for (int j = 0; j < ip_column2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < ip_row2; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    *return_result = result[0][0];
}

/* Ax */
void MatrixVector(double m1[][mv_column1], double m2[][mv_column2], double result[][mv_column2]) {
    if (mv_column1 != mv_row2) { printf("[Caution!] Couldn't define the Product\n"); }

    for (int i = 0; i < mv_row1; i++) {
        for (int j = 0; j < mv_column2; j++) {
            for (int k = 0; k < mv_row2; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/*  */
void param_update_3(double x[dim][1], double f, double Theta[dim][1], double gamma[dim][dim], ReastsquareMethod* rls) {
    double update_k = 0.0, update_d = 0.0, update_c = 0.0;
    double xT[1][dim] = { 0.0 };
    double vec_buf = 0.0, gamma_buf = 0.0;

    for (int i = 0; i < dim; i++) {
        xT[0][i] = x[i][0];
    }

    /* Denominator calculation */
        // gamma*x 
    double mv_result[dim][1] = { 0.0 };
    MatrixVector(gamma, x, mv_result);
    // xT*gammma
    double ip_result = 0.0;
    InnerProduct(xT, mv_result, &ip_result);
    rls->den = rls->lambda + ip_result;

    // recursive error
    for (int i = 0; i < dim; i++) {
        vec_buf += Theta[i][0] * x[i][0];
    }
    rls->epsilon = f - vec_buf;

    /* update law */
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            gamma_buf += gamma[i][j] * x[j][0];
        }
        Theta[i][0] += gamma_buf * rls->epsilon / rls->den;
        gamma_buf = 0.0;
    }
}

/*  */
void gamma_update_3(double x[dim][1], double gamma[dim][dim], ReastsquareMethod* rls) {
    double xT[1][dim] = { 0.0 };
    double den = 0.0;

    for (int i = 0; i < dim; i++) {
        xT[0][i] = x[i][0];
    }

    /* Numerator calculation */
        // x*xT
    double mm_result[dim][dim] = { 0.0 };
    InnerProduct_inv(x, xT, mm_result);
    // (x*xT)*gamma
    double mm_result_2[dim][dim] = { 0.0 };
    MatrixMatrix(mm_result, gamma, mm_result_2);
    // gamma*(x*xT*gamma)
    double mm_result_3[dim][dim] = { 0.0 };
    MatrixMatrix(gamma, mm_result_2, mm_result_3);

    /* Gamma Update */
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            gamma[i][j] = (1 / rls->lambda) * ((gamma[i][j]) - mm_result_3[i][j] / rls->den);
        }
    }
}

/* Variable */
void forgetting_rate_update(ReastsquareMethod* rls) {
    rls->gzi = rls->den - rls->lambda;
    rls->mu = 1 - rls->gzi - pow(rls->epsilon, 2.0) / rls->I;
    rls->lambda = 0.50 * (rls->mu + sqrt(pow(rls->mu, 2.0) + 4.0 * rls->gzi));
}
