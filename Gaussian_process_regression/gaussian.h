/*  */

/* プロトタイプ宣言 */
// ガウス過程回帰
double Delta(double, double);
double Kernel_function(double, double, double[PARAM][1]);
void Compute_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double K[TRAIN_SIZE][TRAIN_SIZE]);
void GPR(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[N][1], double[N][1], double[1][1]);
// ハイパーパラメータの最適化
double objective_function(double[PARAM][1]);
void gradient(double*, double*);
void lbfgs(double*);


// デルタ関数 (同じ数なら1を返す)
double Delta(double x, double xd) {
    if (x = xd) {
        return 1;
    }
    else {
        return 0;
    }
}


// カーネル関数（RBFカーネル + 線形項）
double Kernel_function(double x1, double x2, double theta[PARAM][1]) {
    double diff = x1 - x2;
    double delta = Delta(x1, x2);
    double gauss = theta[0][0] * exp(-diff * diff / theta[1][0]) + theta[2][0] * delta;
    //printf("diff=%lf, gauss=%lf\n", diff, gauss);
    return gauss;
}


// ガウス過程回帰
void GPR(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double test_x[N][1], double test_y[N][1], double var[1][1], double theta[PARAM][1]) {
    double K[TRAIN_SIZE][TRAIN_SIZE];           // 訓練データのカーネル行列
    double K_inv[TRAIN_SIZE][TRAIN_SIZE];       // 逆行列（ここでは簡略化）
    double alpha[TRAIN_SIZE][1] = { 0.0 };      // alpha = K^-1 * y
    double k_star[TRAIN_SIZE][1];               // テストデータと訓練データのカーネル値

    // 訓練データのカーネル行列を計算
    Compute_kernel_matrix(train_x, theta, K);

    // 逆行列
    inverse_matrix(K, K_inv);

    // alpha = K^-1 * y
    MatrixVector(K_inv, train_y, alpha);

    // テストデータに対する予測
    for (int t = 0; t < N; t++) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            k_star[i][0] = Kernel_function(train_x[i][0], test_x[t][0],theta);
            //printf("k*=%lf\n", k_star[i]);
        }

        // 予測値を計算
        test_y[t][0] = 0.0;
        for (int i = 0; i < TRAIN_SIZE; i++) {
            test_y[t][0] += k_star[i][0] * alpha[i][0];
        }

        // 分散の計算
        var[0][0] = Kernel_function(test_x[0][0], test_x[0][0], theta) - xTAx(k_star, K_inv);
        //printf("kernel=%lf\n", Kernel_function(test_x[0][0], test_x[0][0]));
        //printf("xTAx=%lf\n", xTAx(test_y, K));
    }
}

// カーネル行列を計算
void Compute_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double K[TRAIN_SIZE][TRAIN_SIZE]) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < TRAIN_SIZE; j++) {
            K[i][j] = Kernel_function(train_x[i][0], train_x[j][0], params);
            if (i == j) {
                K[i][j] += NOISE_VAR;   // ノイズの分散を加える for 逆行列演算の安定化
            }
        }
    }
}


// 対数尤度を計算 (これを最大化したい)
double compute_log_likelihood(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1]) {
    double K[TRAIN_SIZE][TRAIN_SIZE];
    Compute_kernel_matrix(train_x, params, K);

    double det = 1.0;
    det = Determinant(K);
    if (det < 1e-10) return -1e10;

    double log_det = log(det);
    double log_likelihood = -0.50 * log_det;
    return log_likelihood;
}


// 勾配を計算
//void compute_gradient(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1], double grad[PARAM]) {
//    double epsilon = 1e-5;
//    double log_likelihood_original = compute_log_likelihood(train_x, train_y, params);
//
//    for (int i = 0; i < PARAM; i++) {
//        double params_new[PARAM][1];
//        for (int j = 0; j < PARAM; j++) params_new[j][0] = params[j][0];
//        params_new[i][0] += epsilon;
//
//        double log_likelihood_new = compute_log_likelihood(train_x, train_y, params_new);
//        grad[i] = (log_likelihood_new - log_likelihood_original) / epsilon;
//    }
//}
void compute_gradient(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1], double grad[PARAM]) {
    double epsilon = 1e-5;
    double log_likelihood_original = compute_log_likelihood(train_x, train_y, params);

    for (int i = 0; i < PARAM; i++) {
        double params_new[PARAM][1];
        for (int j = 0; j < PARAM; j++) params_new[j][0] = params[j][0];
        params_new[i][0] += epsilon;

        double log_likelihood_new = compute_log_likelihood(train_x, train_y, params_new);
        grad[i] = (log_likelihood_new - log_likelihood_original) / epsilon;
    }
}

// 最急降下法 (GD)
void Gradient_decent() {

}

// L-BFGS 最適化
void lbfgs_optimize(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1]) {
    double s[M][PARAM], y[M][PARAM], rho[M], q[PARAM], alpha[M], beta;
    double grad[PARAM], prev_grad[PARAM], prev_params[PARAM][1];

    int iter = 0, k = 0;
    compute_gradient(train_x, train_y, params, grad);

    while (iter < MAX_ITER) {
        double grad_norm = 0.0;
        for (int i = 0; i < PARAM; i++) grad_norm += grad[i] * grad[i];
        grad_norm = sqrt(grad_norm);

        //printf("Iteration %d: Gradient Norm = %.8f\n", iter, grad_norm);
        if (grad_norm < EPSILON) break;

        for (int i = 0; i < PARAM; i++) q[i] = -grad[i];

        int m_k = (k < M) ? k : M;
        for (int i = m_k - 1; i >= 0; i--) {
            rho[i] = 1.0 / (s[i][0] * y[i][0] + s[i][1] * y[i][1] + s[i][2] * y[i][2]);
            alpha[i] = rho[i] * (s[i][0] * q[0] + s[i][1] * q[1] + s[i][2] * q[2]);
            for (int j = 0; j < PARAM; j++) q[j] -= alpha[i] * y[i][j];
        }

        for (int i = 0; i < m_k; i++) {
            beta = rho[i] * (y[i][0] * q[0] + y[i][1] * q[1] + y[i][2] * q[2]);
            for (int j = 0; j < PARAM; j++) q[j] += s[i][j] * (alpha[i] - beta);
        }

    /* 更新 */
        double alpha_step = 0.10;   // 学習率？
        for (int i = 0; i < PARAM; i++) {
            prev_params[i][0] = params[i][0];
            prev_grad[i] = grad[i];
            params[i][0] += alpha_step * q[i];
        }

        compute_gradient(train_x, train_y, params, grad);

        for (int i = 0; i < PARAM; i++) {
            s[k % M][i] = params[i][0] - prev_params[i][0];
            y[k % M][i] = grad[i] - prev_grad[i];
        }

        k++;
        iter++;
    }

    //printf("最適化終了 (反復回数: %d)\n", iter);
    //printf("最適ハイパーパラメータ: θ1=%.6f, θ2=%.6f, θ3=%.6f\n", params[0], params[1], params[2]);
}