/*
ガウス過程回帰と機械学習
重回帰分析
1.2 重回帰とベクトル表現
p.26をC言語で実装
*/

#include <stdio.h>
#include <math.h>

#include <iostream>
#include <random>
#include <fstream>

// データ次元と回帰に使うデータ数を指定
#define dim 3
#define datanum 4

// 用いる関数
void Transpose_lsm(double[datanum][dim], double[dim][datanum]);
void Inverse(double X[dim][dim], double Xinv[dim][dim]);
void MatrixVector(double m1[dim][dim], double m2[dim][1], double result[dim][1]);
void MatrixVector_lsm(double m1[dim][datanum], double m2[datanum][1], double result[dim][1]);
void SUM(double A[dim][dim], double B[dim][dim]);
void LSM(double X[datanum][dim], double Y[datanum][1], double Theta[dim][1], int Ridge, double alpha);

/* 本文 */
int main(void) {
    /* リアルタイムLSMのチェック */
    // 推定パラメータ
    double Theta[dim][1] = { 0.0 };
    double X1, X2, X3 = 0.0;
    // 応答値
    double x_res[12][2] = {          1.0,      2.0,
                                    -1.0,     1.0,
                                    3.0,      0.0,
                                    -2.0,     -2.0,
        1.0,      2.0,
                                    -1.0,     1.0,
                                    3.0,      0.0,
                                    -2.0,     -2.0,
                                    1.010,      2.010,
                                    -0.999,     1.010,
                                    3.010,      0.010,
                                    -1.999,     -2.010 };
    double y_res[12][1] = {          4.0,
                                    2.0,
                                    1.0,
                                    -1.0,
        4.0,
                                    2.0,
                                    1.0,
                                    -1.0,
                                    4.010,
                                    1.999,
                                    1.010,
                                    -1.010  };
    int k = 0;

    // 説明変数
    double X_lsm[datanum][dim] = { 0.0 };
    // 目的変数
    double Y_lsm[datanum][1] = { 0.0 };

    // シミュレーション開始
    for (int i = 0; i < 12; i++) {
        printf("xres = [%lf,%lf,%lf], yres = %lf\n", 1.0, x_res[i][0], x_res[i][1], y_res[i][0]);
        k = i % datanum;
        X_lsm[k][0] = 1.0;
        X_lsm[k][1] = x_res[i][0];
        X_lsm[k][2] = x_res[i][1];
        Y_lsm[k][0] = y_res[i][0];
        if (X_lsm[datanum - 1][0] != 0.0) {
            LSM(X_lsm, Y_lsm, Theta, 4, 0.1);
        }
        X1 = Theta[0][0];
        X2 = Theta[1][0];
        X3 = Theta[2][0];
        printf("[X1, X2, X3] = [%lf, %lf, %lf]\n\n", X1, X2, X3);
    }
}

/* ---- 以下、関数定義 ---- */
/* Transpose */
void Transpose_lsm(double x[datanum][dim], double xT[dim][datanum]) {
    for (int i = 0; i < datanum; i++) {
        for (int j = 0; j < dim; j++)
            xT[j][i] = x[i][j];
    }
}

/* Inverse */
void Inverse(double X[dim][dim], double Xinv[dim][dim]) {
    double buf;
    //掃き出し法
    for (int i = 0; i < dim; i++) {
        buf = 1 / X[i][i];
        for (int j = 0; j < dim; j++) {
            X[i][j] *= buf;
            Xinv[i][j] *= buf;
        }
        for (int j = 0; j < dim; j++) {
            if (i != j) {
                buf = X[j][i];
                for (int k = 0; k < dim; k++) {
                    X[j][k] -= X[i][k] * buf;
                    Xinv[j][k] -= Xinv[i][k] * buf;
                }
            }
        }
    }
}

/* Ax */
void MatrixVector(double m1[dim][dim], double m2[dim][1], double result[dim][1]) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < 1; j++) {
            for (int k = 0; k < dim; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

void InnerProduct_inv_lsm(double m1[dim][datanum], double m2[datanum][dim], double result[dim][dim]) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < datanum; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

void MatrixVector_lsm(double m1[dim][datanum], double m2[datanum][1], double result[dim][1]) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < 1; j++) {
            for (int k = 0; k < datanum; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/* Matrix SUM */
void SUM(double A[dim][dim], double B[dim][dim]) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            A[i][j] += B[i][j];
        }
    }
}

/* -- LSM -- */
void LSM(double X[datanum][dim], double Y[datanum][1], double Theta[dim][1], int Ridge, double alpha) {
    double XT[dim][datanum] = { 0.0 };

    for (int i = 0; i < dim; i++) {
        Theta[i][0] = 0.0;
    }

    Transpose_lsm(X, XT);
    //転置取れたかチェック
    printf("XT=\n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < datanum; j++) {
            printf(" %f", XT[i][j]);
        }
        printf("\n");
    }
    printf("Y=\n");
    for (int i = 0; i < datanum; i++) {
        printf(" %f\n", Y[i][1]);
    }
    

    double XTY[dim][1] = { 0.0 };
    double XTX[dim][dim] = { 0.0 };
    double XTXinv[dim][dim] = { 0.0 };
    for (int i = 0; i < dim; i++) { XTXinv[i][i] = 1.0; }
    // 正則化項
    double alphaI[dim][dim] = { 0.0 };
    for (int i = 0; i < dim; i++) { alphaI[i][i] = alpha; }

    // x*xT
    InnerProduct_inv_lsm(XT, X, XTX);
    // 掛け算できたかチェック
    printf("XTX=\n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf(" %f", XTX[i][j]);
        }
        printf("\n");
    }

    // (x*xT)^-1
    Inverse(XTX, XTXinv);
    // 逆行列取れたかチェック
    printf("XTXinv=\n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf(" %f", XTXinv[i][j]);
        }
        printf("\n");
    }

    // XT*y
    MatrixVector_lsm(XT, Y, XTY);
    // 掛け算できたかチェック
    printf("XTY=\n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < 1; j++) {
            printf(" %f", XTY[i][j]);
        }
        printf("\n");
    }

    // (x*xT)^-1 * XT*y
    MatrixVector(XTXinv, XTY, Theta);
    // 掛け算できたかチェック
    printf("Theta=\n");
    for (int i = 0; i < dim; i++) {
            printf(" %f\n", Theta[i][0]);
    }
}