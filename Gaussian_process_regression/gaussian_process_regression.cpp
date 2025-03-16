/* 時系列データに対するガウス過程回帰の練習プログラム */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "definition.h"
#include "matrix.h"
#include "gaussian.h"
#include "gnuplot.h"

/* 本文 */
int main(void) {
    // ファイル書き込み
    FILE* fp{};
    errno_t error;
    // 書き込みファイル
    char data_name[99] = "data/gpr.dat";
    error = fopen_s(&fp, data_name, "w");

    static int i = 0;
    static double t = 0.0;
    static double t_end = 5.0;

    /* ガウス過程回帰 */
    // Alhgorithm
    double K[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    double K_inv[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    
    // Train
    static double K_dat[TRAIN_SIZE][1] = { 0.10, 0.50, 2.0, 5.0, 7.50 };       // (kN/m)
    static double x_dat[TRAIN_SIZE][1] = { 50.0, 10.0, 0.250, 0.10, 0.06660 }; // (mm)
    static double f_dat[TRAIN_SIZE][1] = { 3, 4, 8, 12, 15 };   // (N)
    static double K_hat[N][1] = { 3.0 };
    
    // Result
    static double fcmd[N][1], xcmd[N][1] = { 0.0 };
    static double var_x[1][1], var_f[1][1] = { 0.0 };

    static double theta_x[PARAM][1] = { 1.0, 1.0, 1.0 };     // ハイパーパラメータの初期値
    static double theta_f[PARAM][1] = { 1.0, 1.0, 1.0 };     // ハイパーパラメータの初期値
    static double theta_grad_x[PARAM][1] = { 0.0 };
    static double theta_grad_f[PARAM][1] = { 0.0 };

    //static double theta_x[PARAM][1] = { 181.22, 10.0, 185.22 };     // ハイパーパラメータの初期値
    //static double theta_f[PARAM][1] = { 181.22, 10.0, 185.22 };     // ハイパーパラメータの初期値

    /* 尤度関数 */
    static double L_posi = 0.0;
    static double L_force = 0.0;

    /* 実時間開始 */
    for (int j = 0; t < t_end; ++j) {
        // 訓練データの生成、ノイズ重畳
        for (i = 0; i < TRAIN_SIZE; i++) {
            if (j % 2 == 0) {
                K_dat[i][0] += (rand() % 100) / 100000.0;
                x_dat[i][0] += (rand() % 100) / 10000.0;
                f_dat[i][0] += (rand() % 100) / 20000.0;
            }
            else {
                K_dat[i][0] -= (rand() % 100) / 100000.0;
                x_dat[i][0] -= (rand() % 100) / 10000.0;
                f_dat[i][0] -= (rand() % 100) / 20000.0;
            }
        }

        // テストデータの生成
        for (i = 0; i < N; i++) {
            K_hat[i][0] = 3.0 + (rand() % 100) / 5000.0;
        }

    /* ガウス過程回帰 */
        // 回帰パート 
        GPR(K_dat, x_dat, K_hat, xcmd, var_x, theta_x);
        GPR(K_dat, f_dat, K_hat, fcmd, var_f, theta_f);

        //// 最適化パート
        //// SCG
        //scg_optimize(K_dat, x_dat, theta_x);
        //scg_optimize(K_dat, f_dat, theta_f);
        // GD
        GD(K_dat, x_dat, theta_x, theta_grad_x);
        GD(K_dat, f_dat, theta_f, theta_grad_f);

        // 尤度関数の計算
        L_posi = compute_log_likelihood(K_dat, x_dat, theta_x);
        L_force = compute_log_likelihood(K_dat, f_dat, theta_f);

        /* データ書き込み */
        fprintf(fp, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
            t,
            K_dat[0][0],    K_dat[1][0],    K_dat[2][0],    K_dat[3][0],    K_dat[4][0],    // 2~6
            x_dat[0][0],    x_dat[1][0],    x_dat[2][0],    x_dat[3][0],    x_dat[4][0],    // 7~11
            f_dat[0][0],    f_dat[1][0],    f_dat[2][0],    f_dat[3][0],    f_dat[4][0],    // 12~16
            K_hat[0][0],    xcmd[0][0],     fcmd[0][0],     var_x[0][0],    var_f[0][0],    // 17~21
            theta_x[0][0],  theta_x[1][0],  theta_x[2][0],                                  // 22~24
            theta_f[0][0],  theta_f[1][0],  theta_f[2][0],                                  // 25~27
            var_x[0][0],    var_f[0][0],                                                    // 28,29
            L_posi,         L_force,                                                        // 30,31
            theta_grad_x[0][0],     theta_grad_x[1][0],     theta_grad_x[2][0],             // 32~34
            theta_grad_x[0][0],     theta_grad_x[1][0],     theta_grad_x[2][0]              // 35~37
        );

        // 結果の表示
        if (j % 1000 == 0) {
            printf("[t=%.2lf] Ke=%.3lf, (fcmd=%.2lf, fvar=%.2lf), (xcmd=%.2lf, xvar=%.2lf)\n", t, K_hat[0][0], fcmd[0][0], var_f[0][0], xcmd[0][0], var_x[0][0]);
            printf("Theta_x = (%.2lf, %.2lf, %.2lf), Lposi = %.2lf\n", theta_x[0][0], theta_x[1][0], theta_x[2][0], L_posi);
            printf("Theta_f = (%.2lf, %.2lf, %.2lf), Lforce = %.2lf\n", theta_f[0][0], theta_f[1][0], theta_f[2][0], L_force);
        }
        t += ST;
    }

    fclose(fp);
    Gnuplot(data_name, t_end);

    return 0;
}
