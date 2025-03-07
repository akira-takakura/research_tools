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
    //// ファイル書き込み
    //FILE* fp{};
    //errno_t error;
    //// 書き込みファイル
    //char data_name[99] = "data/gpr.dat";
    //error = fopen_s(&fp, data_name, "w");

    //static int i = 0;
    //static double t = 0.0;
    //static double t_end = 5.0;

    ///* ガウス過程回帰 */
    //// Alhgorithm
    //double K[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    //double K_inv[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    //
    //// Train
    //static double K_dat[TRAIN_SIZE][1] = { 0.1, 0.5, 2.0, 5.0, 7.50 };       // (kN/m)
    //static double x_dat[TRAIN_SIZE][1] = { 50.0, 10.0, 0.250, 0.10, 0.06660 }; // (mm)
    //static double f_dat[TRAIN_SIZE][1] = { 3, 4, 8, 12, 15 };   // (N)
    //static double K_hat[N][1] = { 3.0 };
    //
    //// Result
    //static double fcmd[N][1], xcmd[N][1] = { 0.0 };
    //static double var_x[1][1], var_f[1][1] = { 0.0 };

    //static double theta_x[PARAM][1] = { 1.0, 1.0, 1.0 };     // ハイパーパラメータの初期値
    //static double theta_f[PARAM][1] = { 1.0, 1.0, 1.0 };     // ハイパーパラメータの初期値

    ////static double theta_x[PARAM][1] = { 181.22, 10.0, 185.22 };     // ハイパーパラメータの初期値
    ////static double theta_f[PARAM][1] = { 181.22, 10.0, 185.22 };     // ハイパーパラメータの初期値

    ///* 尤度関数 */
    //static double L_posi = 0.0;
    //static double L_force = 0.0;

    ///* 実時間開始 */
    //for (int j = 0; t < t_end; ++j) {
    //    // 訓練データの生成、ノイズ重畳
    //    for (i = 0; i < TRAIN_SIZE; i++) {
    //        if (j % 2 == 0) {
    //            K_dat[i][0] += (rand() % 100) / 20000.0;
    //            x_dat[i][0] += (rand() % 100) / 2000.0;
    //            f_dat[i][0] += (rand() % 100) / 20000.0;
    //        }
    //        else {
    //            K_dat[i][0] -= (rand() % 100) / 20000.0;
    //            x_dat[i][0] -= (rand() % 100) / 2000.0;
    //            f_dat[i][0] -= (rand() % 100) / 20000.0;
    //        }
    //    }

    //    // テストデータの生成
    //    for (i = 0; i < N; i++) {
    //        K_hat[i][0] = 3.0 + (rand() % 100) / 5000.0;
    //    }

    ///* ガウス過程回帰 */
    //    // 回帰パート 
    //    GPR(K_dat, x_dat, K_hat, fcmd, var_x, theta_x);
    //    GPR(K_dat, f_dat, K_hat, xcmd, var_f, theta_f);

    //    //// 最適化パート
    //    // L-BFGS
    //    //lbfgs_optimize(K_dat, f_dat, theta_x);
    //    //lbfgs_optimize(K_dat, x_dat, theta_f);
    //    //// SCG
    //    //scg_optimize(K_dat, x_dat, theta_x);
    //    //scg_optimize(K_dat, f_dat, theta_f);
    //    // GD
    //    GD(K_dat, x_dat, theta_x);
    //    GD(K_dat, f_dat, theta_f);

    //    // 尤度関数の計算
    //    L_posi = compute_log_likelihood(K_dat, x_dat, theta_x);
    //    L_force = compute_log_likelihood(K_dat, f_dat, theta_f);

    //    /* データ書き込み */
    //    fprintf(fp, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
    //        t,
    //        K_dat[0][0],    K_dat[1][0],    K_dat[2][0],    K_dat[3][0],    K_dat[4][0],    // 2~6
    //        x_dat[0][0],    x_dat[1][0],    x_dat[2][0],    x_dat[3][0],    x_dat[4][0],    // 7~11
    //        f_dat[0][0],    f_dat[1][0],    f_dat[2][0],    f_dat[3][0],    f_dat[4][0],    // 12~16
    //        K_hat[0][0],    xcmd[0][0],     fcmd[0][0],     var_x[0][0],    var_f[0][0],    // 17~21
    //        theta_x[0][0],  theta_x[1][0],  theta_x[2][0],                                  // 22~24
    //        theta_f[0][0],  theta_f[1][0],  theta_f[2][0],                                  // 25~27
    //        var_x[0][0],    var_f[0][0],                                                    // 28,29
    //        L_posi,         L_force                                                         // 30,31
    //    );

    //    // 結果の表示
    //    if (j % 1000 == 0) {
    //        printf("[t=%lf] Ke=%.2lf, (fcmd=%.2lf, fvar=%.2lf), (xcmd=%.2lf, xvar=%.2lf)\n", t, K_hat[0][0], fcmd[0][0], var_f[0][0], xcmd[0][0], var_x[0][0]);
    //        printf("Theta_x = (%.2lf, %.2lf, %.2lf)\n", theta_x[0][0], theta_x[1][0], theta_x[2][0]);
    //        printf("Theta_f = (%.2lf, %.2lf, %.2lf)\n", theta_f[0][0], theta_f[1][0], theta_f[2][0]);
    //        printf("Likelihood (Lposi = %.2lf, Lforce = %.2lf)\n", L_posi, L_force);
    //    }
    //    t += ST;
    //}

    //fclose(fp);
    //Gnuplot(data_name, t_end);


    /* -------------------------------------------------------------------------------------------------------------------------------------------------- */
    /* 第2部 */
    /*
    let's 青本p.91再現
    -0.5 0.7
    0.5	 1.8
    1	 1.7
    1.4	 2.3
    3	 1

    2.3	 0
    2.5	 0.2
    1.5	 2
    1.1	 2.4
    0.7	 1.5
    */

    //// Train
    static double train_x[TRAIN_SIZE][1] = { -0.50, 0.50, 1.0, 1.4, 3.0 };
    static double train_y[TRAIN_SIZE][1] = { 0.70, 1.80, 1.70, 2.30, 1.0 };
    //static double theta[PARAM][1] = { 1.5960, 6.560, 0.0820 };     // ハイパーパラメータの初期値
    static double theta[PARAM][1] = { 1.0, 5.0, 0.10 };     // ハイパーパラメータの初期値
    
    //static double train_x[TRAIN_SIZE][1] = { -0.50, 0.50, 1.0, 1.4, 3.0, 2.30, 2.50, 1.50, 1.10, 0.70 };
    //static double train_y[TRAIN_SIZE][1] = { 0.70, 1.80, 1.70, 2.30, 1.0, 0.0, 0.20, 2.0, 2.40, 1.50 };
    //static double theta[PARAM][1] = { 1.5250, 0.6890, 0.0670 };     // ハイパーパラメータの初期値

    static double L = 0.0;


    printf("Init theta=[%lf, %lf, %lf]\n", theta[0][0], theta[1][0], theta[2][0]);
    for (int i = 0; i < 100; i++) {
        L = compute_log_likelihood(train_x, train_y, theta);

        GD(train_x, train_y, theta);


        printf("%lf\n", L);
        printf("theta=[%lf, %lf, %lf]\n", theta[0][0], theta[1][0], theta[2][0]);
    }

    return 0;
}
