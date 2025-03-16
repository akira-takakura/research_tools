/* ガウス過程回帰の練習プログラム */
// ハイパーパラメータ決定法　https://qiita.com/meltyyyyy/items/5a058ecc81e010876a39
// 青本の解説　http://chasen.org/~daiti-m/gpbook/
// CAE活用のための不確かさの定量化 ―ガウス過程回帰と実験計画法を用いたサロゲートモデリング―
// https://kesco.co.jp/blog/3868/、例題あり、分かりやすい！！！

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "definition.h"
#include "matrix.h"
#include "gaussian.h"
#include "optimization.h"
#include "gnuplot.h"


/* 本文 */
int main(void) {
    // ファイル書き込み
    FILE* fp{};
    errno_t error;
    // 書き込みファイル
    char data_name[99] = "data/gpr_test.dat";
    error = fopen_s(&fp, data_name, "w");

    static double t = 0.0;
    const double dt = 0.010;
    const double t_end = 1.0;
    
/* Gaussian */
/* Train data */    
    // DATA1 (L=-1.788)
    static double   train_x[TRAIN_SIZE][1]  = { -0.50, 0.50, 1.0, 1.4, 3.0 };
    static double   train_y[TRAIN_SIZE][1]  = { 0.70, 1.80, 1.70, 2.30, 1.0 };
    //static double    theta[PARAM][1] = { 1.5960, 6.560, 0.0820 };           // ハイパーパラメータの初期値が真値
    static double   theta[PARAM][1]         = { 1, 1, 1 };               // ハイパーパラメータの初期値
    const double    theta_true[PARAM][1]    = { 1.5960, 6.560, 0.0820 };        // ハイパーパラメータの真値
    static double   theta_grad[PARAM][1]    = { 0.0 };
    
    //// DATA2 (L=-2.174)
    //static double train_x[TRAIN_SIZE][1] = { -0.50, 0.50, 1.0, 1.4, 3.0, 2.30, 2.50, 1.50, 1.10, 0.70 };
    //static double train_y[TRAIN_SIZE][1] = { 0.70, 1.80, 1.70, 2.30, 1.0, 0.0, 0.20, 2.0, 2.40, 1.50 };
    ////static double theta[PARAM][1] = { 1.5250, 0.6890, 0.0670 };     // ハイパーパラメータの初期値が真値
    //static double theta[PARAM][1] = { 0.1, 0.1, 0.1 };     // ハイパーパラメータの初期値
    //const double theta_true[PARAM][1] = { 1.5250, 0.6890, 0.0670 };     // ハイパーパラメータの真値
    //static double   theta_grad[PARAM][1] = { 0.0 };

    //// DATA3 (L=-5.9189)
    ////https://kesco.co.jp/blog/3868/
    //static double train_x[TRAIN_SIZE][1] = { -3.50, -1.50, 0.0, 0.5, 3.0 };
    //static double train_y[TRAIN_SIZE][1] = { 0.70, 1.80, 1.60, 2.30, 1.0 };
    ////static double theta[PARAM][1] = { 0.400, 3.225, -1.950 };       // ハイパーパラメータ.596の初期値が真値 (log)
    ////static double theta[PARAM][1] = { 1.4918, 25.1536, 0.14227 };     // ハイパーパラメータの初期値が真値
    ////static double theta[PARAM][1] = { 1.0, 1.0, 0.010 };             // ハイパーパラメータの初期値 (example)
    //static double theta[PARAM][1] = { 1.0, 25.1536, 0.14227 };             // ハイパーパラメータの初期値
    ////const double theta_true[PARAM][1] = { 0.400, 3.225, -1.950 };     // ハイパーパラメータの真値 (log)
    //const double theta_true[PARAM][1] = { 1.4918, 25.1536, 0.14227 };   // ハイパーパラメータの真値
    //static double   theta_grad[PARAM][1]    = { 0.0 };

/* Log likelihood */
    static double L = 0.0;

    printf("Init theta=[%lf, %lf, %lf]\n", theta[0][0], theta[1][0], theta[2][0]);

    // 主ループ
    for (int i = 0; t < t_end; i++) {
        L = compute_log_likelihood(train_x, train_y, theta);

        GD(train_x, train_y, theta, theta_grad);
        //CGD(train_x, train_y, theta, theta_grad);
        //SCGD(train_x, train_y, theta, theta_grad);

        /* データ書き込み */
        fprintf(fp, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
            t, L,
            theta[0][0] - theta_true[0][0],     // 3
            theta[1][0] - theta_true[1][0], 
            theta[2][0] - theta_true[2][0], 
            theta_grad[0][0],                   // 6
            theta_grad[1][0],
            theta_grad[2][0], 
            theta[0][0],                        // 9
            theta[1][0],
            theta[2][0]
        );

        if (i % 1 == 0) {
            //printf("%lf\n", L);
            printf("[t=%.2lf] Theta=[%lf, %lf, %lf], L=%lf\n", t, theta[0][0], theta[1][0], theta[2][0], L);
            //printf("[t=%.2lf] Theta_grad=[%lf, %lf, %lf]\n", t, theta_grad[0][0], theta_grad[1][0], theta_grad[2][0]);
        }
        t += dt;
    }

    fclose(fp);
    Gnuplot(data_name, t_end);
    return 0;
}