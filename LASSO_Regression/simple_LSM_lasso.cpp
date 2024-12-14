#include <stdio.h>
#include <math.h>

#define datanum 4   // データ点数
#define dim 3        // 説明変数の次元

// ラッソ回帰のメイン関数
void Lasso(double X[datanum][dim], double Y[datanum][1], double Theta[dim][1], double lambda, double learning_rate, int iterations) {
    // 重みの初期化
    for (int i = 0; i < dim; i++) {
        Theta[i][0] = 0.0;
    }
    double bias = 0.0;           // バイアスの初期化
    //転置取れたかチェック
    printf("X=\n");
    for (int i = 0; i < datanum; i++) {
        for (int j = 0; j < dim; j++) {
            printf(" %f", X[i][j]);
        }
        printf("\n");
    }
    printf("Y=\n");
    for (int i = 0; i < datanum; i++) {
        printf(" %f\n", Y[i][0]);
    }

    for (int iter = 0; iter < iterations; iter++) {
        double weight_gradients[dim] = {0.0};
        double bias_gradient = 0.0;

        // 勾配の計算
        for (int i = 0; i < datanum; i++) {
            double prediction = bias;
            for (int j = 0; j < dim; j++) {
                prediction += Theta[j][0] * X[i][j];
            }
            double error = prediction - Y[i][0];

            // 勾配の累積
            for (int j = 0; j < dim; j++) {
                weight_gradients[j] += error * X[i][j];
            }
            bias_gradient += error;
        }

        // パラメータの更新 (L1 正則化を考慮)
        for (int j = 0; j < dim; j++) {
            double grad = weight_gradients[j] / datanum;
            if (Theta[j][0] > 0) {
                grad += lambda;
            } else if (Theta[j][0] < 0) {
                grad -= lambda;
            }
            Theta[j][0] -= learning_rate * grad;
        }
        bias -= learning_rate * (bias_gradient / datanum);

        // 経過表示（オプション）
        if (iter % 100 == 0) {
            printf("Iteration %d: Theta = [", iter);
            for (int j = 0; j < dim; j++) {
                printf("%.4f ", Theta[j][0]);
            }
            printf("], Bias = %.4f\n", bias);
        }
    }

    // 最終結果を表示
    printf("Final Parameters:\n");
    printf("Theta: [");
    for (int j = 0; j < dim; j++) {
        printf("%.4f ", Theta[j]);
    }
    printf("]\n");
    printf("Bias: %.4f\n", bias);
}

// テスト用のメイン関数
int main() {
    /* リアルタイムLSMのチェック */
    // 推定パラメータ
    double Theta[dim][1] = { 0.0 };
    double X1, X2, X3 = 0.0;
    // 応答値
    double x_res[12][2] = { 1.0,      2.0,
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
    double y_res[12][1] = { 4.0,
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
                                    -1.010 };
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
            Lasso(X_lsm, Y_lsm, Theta, 0.10, 0.010, 1000);
        }
        X1 = Theta[0][0];
        X2 = Theta[1][0];
        X3 = Theta[2][0];
        printf("[X1, X2, X3] = [%lf, %lf, %lf]\n\n", X1, X2, X3);
    }
}
