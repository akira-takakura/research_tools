/* ガウス過程回帰の練習プログラム */
// ハイパーパラメータ決定法　https://qiita.com/meltyyyyy/items/5a058ecc81e010876a39
// 青本の解説　http://chasen.org/~daiti-m/gpbook/
// CAE活用のための不確かさの定量化 ―ガウス過程回帰と実験計画法を用いたサロゲートモデリング―

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
    printf("[RUNNING] mode 'Reproduction'\n");
    error = fopen_s(&fp, data_name, "w");

    static int i = 0;
    static double t = 0.0;

    static double K_dat[TRAIN_SIZE][1] = { 100, 500, 2000, 5000, 7500 };       // (N/m)
    static double x_dat[TRAIN_SIZE][1] = { 50.0, 10.0, 0.250, 0.10, 0.06660 }; // (mm)
    static double f_dat[TRAIN_SIZE][1] = { 3000, 4000, 8000, 12000, 15000 };   // (mN)
    static double K_hat[N][1] = { 3000.0 };
    static double fcmd[N][1], xcmd[N][1] = { 0.0 };
    static double var_x[1][1], var_f[1][1] = { 0.0 };

    static double theta_x[PARAM][1] = { 1.0, 1.0, 1.0 };     // ハイパーパラメータの初期値
    static double theta_f[PARAM][1] = { 1.0, 1.0, 1.0 };     // ハイパーパラメータの初期値

    //static double theta_x[PARAM][1] = { 181.22, 10.0, 185.22 };     // ハイパーパラメータの初期値
    //static double theta_f[PARAM][1] = { 181.22, 10.0, 185.22 };     // ハイパーパラメータの初期値

    /* 実時間開始 */
    for (int j = 0; t < 1.00; ++j) {

        // 訓練データの生成、ノイズ重畳
        for (i = 0; i < TRAIN_SIZE; i++) {
            if (j % 2 == 0) {
                K_dat[i][0] += (rand() % 100) / 10.0;
                x_dat[i][0] += (rand() % 100) / 1000.0;
                f_dat[i][0] += (rand() % 100) / 5.0;
            }
            else {
                K_dat[i][0] -= (rand() % 100) / 10.0;
                x_dat[i][0] -= (rand() % 100) / 1000.0;
                f_dat[i][0] -= (rand() % 100) / 5.0;
            }
        }

        // テストデータの生成
        for (i = 0; i < N; i++) {
            K_hat[i][0] = 3000 + (rand() % 100) / 10.0;
        }

    /* ガウス過程回帰 */
        
        // 回帰パート 
        GPR(K_dat, f_dat, K_hat, fcmd, var_f, theta_x);
        GPR(K_dat, x_dat, K_hat, xcmd, var_x, theta_f);

        // 最適化パート
        lbfgs_optimize(K_dat, f_dat, theta_x);
        lbfgs_optimize(K_dat, x_dat, theta_f);

        /* データ書き込み */
        fprintf(fp, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
            t,
            K_dat[0][0],   K_dat[1][0], K_dat[2][0],    K_dat[3][0],    K_dat[4][0],    // 2~6
            x_dat[0][0],   x_dat[1][0], x_dat[2][0],    x_dat[3][0],    x_dat[4][0],    // 7~11
            f_dat[0][0],   f_dat[1][0], f_dat[2][0],    f_dat[3][0],    f_dat[4][0],    // 12~16
            K_hat[0][0],   xcmd[0][0],  fcmd[0][0],     var_x[0][0],    var_f[0][0]     // 17~21
        );

        // 結果の表示
        if (j % 1000 == 0) {
            printf("[t=%lf] Ke=%.0lf, (fcmd=%.2lf, fvar=%.2lf), (xcmd=%.2lf, xvar=%.2lf)\n", t, K_hat[0][0], fcmd[0][0], var_f[0][0], xcmd[0][0], var_x[0][0]);
            printf("Theta_x = (%.2lf, %.2lf, %.2lf)\n", theta_x[0][0], theta_x[1][0], theta_x[2][0]);
            printf("Theta_f = (%.2lf, %.2lf, %.2lf)\n", theta_f[0][0], theta_f[1][0], theta_f[2][0]);
        }
        t += ST;
    }

    fclose(fp);
    Gnuplot(data_name);
    return 0;
}
