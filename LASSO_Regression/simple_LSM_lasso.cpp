#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define datanum 1000        // データ点の数
#define dim 3               // 説明変数の次元
#define learning_rate 0.010 // 学習率
#define lambda 0.0010       // 正則化係数
#define iterations 1000     // 反復回数

// 関数プロトタイプ
void Lasso(double X[datanum][dim], double Y[datanum][1], double Theta[dim + 1][1]);

// ラッソ回帰関数
void Lasso(double X[datanum][dim], double Y[datanum][1], double Theta[dim + 1][1]) {
    for (int i = 0; i < dim + 1; i++) {
        Theta[i][0] = 0.0; // パラメータの初期化
    }

    for (int iter = 0; iter < iterations; iter++) {
        double gradients[dim + 1] = { 0.0 }; // 勾配を初期化

        // 勾配の計算
        for (int i = 0; i < datanum; i++) {
            double prediction = Theta[0][0]; // バイアス項
            for (int j = 0; j < dim; j++) {
                prediction += Theta[j + 1][0] * X[i][j];
            }
            double error = prediction - Y[i][0];
            gradients[0] += error; // バイアス項の勾配
            for (int j = 0; j < dim; j++) {
                gradients[j + 1] += error * X[i][j];
            }
        }

        // パラメータの更新
        for (int j = 0; j < dim + 1; j++) {
            if (j > 0) { // 説明変数の係数（Theta[1]以降）に正則化項を適用
                if (Theta[j][0] > 0) {
                    gradients[j] += lambda; // L1ノルムのペナルティ
                }
                else if (Theta[j][0] < 0) {
                    gradients[j] -= lambda;
                }
            }
            Theta[j][0] -= learning_rate * gradients[j] / datanum;
        }

        // 進捗の表示（オプション）
        if (iter % 100 == 0) {
            printf("Iteration %d: ", iter);
            for (int j = 0; j < dim + 1; j++) {
                printf("Theta[%d]=%.4f ", j, Theta[j][0]);
            }
            printf("\n");
        }
    }
}

// 主関数
int main() {
    // サンプルデータの準備
    double X[datanum][dim];
    double Y[datanum][1];
    double Theta[dim + 1][1];

    // データの初期化（例としてランダムデータを使用）
    for (int i = 0; i < datanum; i++) {
        for (int j = 0; j < dim; j++) {
            X[i][j] = (double)rand() / RAND_MAX * 10.0; // 0~10のランダム値
        }
        Y[i][0] = 3.0 * X[i][0] + 2.0 * X[i][1] - X[i][2] + 5.0 + ((double)rand() / RAND_MAX - 0.5); // ノイズを加えた線形データ
    }

    // ラッソ回帰の実行
    Lasso(X, Y, Theta);

    // 結果の表示
    printf("\nFinal estimated parameters:\n");
    for (int j = 0; j < dim + 1; j++) {
        printf("Theta[%d] = %.4f\n", j, Theta[j][0]);
    }

    return 0;
}
