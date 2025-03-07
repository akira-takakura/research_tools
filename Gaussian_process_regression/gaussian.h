/*  */

/* プロトタイプ宣言 */
// ガウス過程回帰
double Delta(double, double);
double Delta_int(int, int);
double Kernel_function(double, double, double[PARAM][1]);
void Compute_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double K[TRAIN_SIZE][TRAIN_SIZE]);
void Compute_partial_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double K[TRAIN_SIZE][TRAIN_SIZE],int);
void GPR(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[N][1], double[N][1], double[1][1]);
// ハイパーパラメータの最適化
double Compute_gradient(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[PARAM][1], double [PARAM][TRAIN_SIZE], int);
void GD(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[PARAM][1]);
void scg_optimize(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[PARAM][1]);
void lbfgs(double*);

// デルタ関数 (同じ数なら1を返す)
double Delta(double x, double xd) {
    if (x == xd) {
        return 1;
    }
    else {
        return 0;
    }
}

// デルタ関数 (同じ数なら1を返す)
double Delta_int(int x, int xd) {
    if (x == xd) {
        return 1.0;
    }
    else {
        return 0.0;
    }
}

// カーネル関数（RBFカーネル + 線形項）
double Kernel_function(double x1, double x2, double theta[PARAM][1]) {
    double diff = x1 - x2;
    double delta = Delta(x1, x2);
    if (diff < 0) { diff *= -1.0; }
    return theta[0][0] * exp(-diff * diff / theta[1][0]) + theta[2][0] * delta;
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

// ガウス過程回帰
void GPR(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double test_x[N][1], double test_y[N][1], double var[1][1], double theta[PARAM][1]) {
    double K[TRAIN_SIZE][TRAIN_SIZE];           // 訓練データのカーネル行列
    double K_inv[TRAIN_SIZE][TRAIN_SIZE];       // 逆行列
    double alpha[TRAIN_SIZE][1] = { 0.0 };      // alpha = K^-1 * y
    double k_star[TRAIN_SIZE][1];               // テストデータと訓練データのカーネル値
    double k_star_T[1][TRAIN_SIZE];             // テストデータと訓練データのカーネル値

    // 訓練データのカーネル行列を計算
    Compute_kernel_matrix(train_x, theta, K);
    inverse_matrix(K, K_inv);

    // alpha = K^-1 * y
    MatrixVector(K_inv, train_y, alpha);

    // テストデータに対する予測
    for (int t = 0; t < N; t++) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            k_star[i][0] = Kernel_function(train_x[i][0], test_x[t][0], theta);
        }

        // 予測値を計算
        //test_y[t][0] = 0.0;
        Transpose(k_star, k_star_T);
        test_y[t][0] = InnerProduct(k_star_T, alpha);

        // 分散の計算
        var[0][0] = Kernel_function(test_x[0][0], test_x[0][0], theta) - xTAx(k_star, K_inv);
    }
}

// 対数尤度を計算 (これを最大化したい)
double compute_log_likelihood(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1]) {
    // min L = ln(K) + y'Ky
    double det = 1.0;
    double K[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    double K_inv[TRAIN_SIZE][TRAIN_SIZE];       // 逆行列
    double yTKy = 0.0;

    Compute_kernel_matrix(train_x, params, K);  // カーネル行列を算出
    det = Determinant(K);
    if (det < 1e-9) return -1e10;  // もし特異行列なら安全に処理
    inverse_matrix(K, K_inv);
    yTKy = xTAx(train_y, K_inv);      // y'* alpha


//// Checker
    //printf("det=%lf\n", det);
    //printf("log(det)=%lf\n", log(det));
    //printf("yTKy=%lf\n", yTKy);
    //
    //static double inv = 0.0;
    //double inv_mat[TRAIN_SIZE][TRAIN_SIZE] = {0.0};       // 逆行列
    //MatrixMatrix(K, K_inv, inv_mat);
    //printf("K=\n");
    //for (int i = 0; i < TRAIN_SIZE; i++) {
    //    for (int j = 0; j < TRAIN_SIZE; j++) {
    //        printf("%lf, ", K_inv[i][j]);
    //    }
    //    printf("\n");
    //}
    
    return  -log(det) - yTKy;
    //return -0.50 * TRAIN_SIZE * log(2.0 * PI) - 0.50 * log(det) - 0.50 * yTKy;
}

// カーネル行列の偏微分を計算
void Compute_partial_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double part_K[TRAIN_SIZE][TRAIN_SIZE], int param) {
    part_K[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    if (param == 0) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            for (int j = 0; j < TRAIN_SIZE; j++) {
                part_K[i][j] = Kernel_function(train_x[i][0], train_x[j][0], params) - params[2][0] * Delta_int(i,j);
                if (i == j) {
                    part_K[i][j] += NOISE_VAR;   // ノイズの分散を加える for 逆行列演算の安定化
                }
            }
        }
    }
    else if (param == 1) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            for (int j = 0; j < TRAIN_SIZE; j++) {
                part_K[i][j] = Kernel_function(train_x[i][0], train_x[j][0], params) - params[2][0] * Delta_int(i,j);
                //printf("\nbefore partK[%d][%d] = %lf\n", i, j, part_K[i][j]);
                 
                part_K[i][j] *= pow(train_x[i][0] - train_x[j][0], 2.0) / params[1][0];     // 指数マイナスなので
                //part_K[i][j] *= pow(train_x[i][0] - train_x[j][0], 2.0) * params[1][0];
                //printf("after partK[%d][%d] = %lf\n", i, j, part_K[i][j]);
                //printf("*= %lf\n", pow(train_x[i][0] - train_x[j][0], 2.0));
                if (i == j) {
                    part_K[i][j] += NOISE_VAR;   // ノイズの分散を加える for 逆行列演算の安定化
                }
            }
        }
    }
    else if (param == 2) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            for (int j = 0; j < TRAIN_SIZE; j++) {
                part_K[i][j] = params[2][0] * Delta_int(i, j);
            }
        }
    }

    //// check
    //printf("\n\npart_K= when param = %d", param);
    //for (int i = 0; i < TRAIN_SIZE; i++) {
    //    printf("\n");
    //    for (int j = 0; j < TRAIN_SIZE; j++) {
    //        printf("%lf ", part_K[i][j]);
    //    }
    //}
}

// 尤度関数の勾配を計算
/* https://qiita.com/meltyyyyy/items/5a058ecc81e010876a39 */
double Compute_gradient(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1], double grad[PARAM][1], int param) {
    double part_K_theta[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    double K_theta[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };
    double K_theta_inv[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };   // 逆行列
    double in_trace[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };      // K^-1 \part{K}
    double first_term, second_term = 0.0;

    double alpha[TRAIN_SIZE][1] = { 0.0 };                  // alpha = K^-1 * y
    double alpha_T[1][TRAIN_SIZE] = { 0.0 };
    double beta[TRAIN_SIZE][1] = { 0.0 };                   // \part{K_theta}*(K^-1y)

    // Preparation
    Compute_partial_kernel_matrix(train_x, params, part_K_theta, param);
    Compute_kernel_matrix(train_x, params, K_theta);
    inverse_matrix(K_theta, K_theta_inv);

    //// check
    //printf("\n\nK_theta_inv=");
    //for (int i = 0; i < TRAIN_SIZE; i++) {
    //    printf("\n");
    //    for (int j = 0; j < TRAIN_SIZE; j++) {
    //        printf("%lf ", K_theta_inv[i][j]);
    //    }
    //}

    // 1st term
    MatrixMatrix(K_theta_inv, part_K_theta,in_trace);
    first_term = Trace(in_trace);
    //printf("\n\n1st term = %lf\n", first_term);
    
    //// check
    //printf("\nin_trace=");
    //for (int i = 0; i < TRAIN_SIZE; i++) {
    //    printf("\n");
    //    for (int j = 0; j < TRAIN_SIZE; j++) {
    //        printf("%lf ", in_trace[i][j]);
    //    }
    //}

    // 2nd term
    MatrixVector(K_theta_inv, train_y, alpha);    // alpha = K^-1 * y
    MatrixVector(part_K_theta, alpha, beta);
    Transpose(alpha, alpha_T);
    second_term = InnerProduct(alpha_T, beta);
    //printf("\n\n2nd term = %lf\n", second_term);

    return first_term + second_term;
}

// 最急降下法(GD)による最適化
void GD(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1]) {
    double grad[PARAM][1] = { 0.0 };
    double alpha = 0.0010;

    // 変換 [tau, sigma, eta]
    double params_log[PARAM][1] = { log(params[0][0]), log(params[1][0]), log(params[2][0]) };

    // 探索 (対数)
    for (int i = 0; i < PARAM; i++) {
        grad[i][0] = Compute_gradient(train_x, train_y, params, grad, i);   // 各パラiに対して方向決定
        params_log[i][0] -= alpha * grad[i][0];                                 // パラメータ更新
        params[i][0] = pow(E, params_log[i][0]);
    }

    //// 探索 (普通)
    //for (int i = 0; i < PARAM; i++) {
    //    grad[i][0] = Compute_gradient(train_x, train_y, params, grad, i);   // 各パラiに対して方向決定
    //    params[i][0] -= alpha * grad[i][0];                                 // パラメータ更新
    //    //params[i][0] += alpha * grad[i][0];                               // パラメータ更新
    //}
}

// SCG法による最適化
void scg_optimize(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1]) {
    double grad[PARAM], prev_grad[PARAM], d[PARAM], s[PARAM];
    double beta, sigma, lambda = 1e-4, alpha, delta, gamma, mu, phi, new_L;
    double L = compute_log_likelihood(train_x, train_y, params);

    //Compute_gradient(train_x, train_y, params, grad, 1);

    for (int i = 0; i < PARAM; i++) {
        d[i] = -grad[i]; // 初期方向
    }

    for (int iter = 0; iter < MAX_ITER; iter++) {
        double grad_norm = 0.0;
        for (int i = 0; i < PARAM; i++) grad_norm += grad[i] * grad[i];
        grad_norm = sqrt(grad_norm);

        //printf("Iteration %d: L = %.8f, Gradient Norm = %.8f\n", iter, L, grad_norm);

        if (grad_norm < EPSILON) break;

        //Compute_gradient(train_x, train_y, params, s);

        sigma = 0.0;
        for (int i = 0; i < PARAM; i++) sigma += d[i] * s[i];

        mu = sigma + lambda * grad_norm;
        phi = -grad_norm / mu;
        for (int i = 0; i < PARAM; i++) params[i][0] += phi * d[i];

        new_L = compute_log_likelihood(train_x, train_y, params);
        delta = 2 * (new_L - L) / (phi * sigma);

        if (delta >= 0) {
            L = new_L;
            //Compute_gradient(train_x, train_y, params, grad);
            lambda *= fmax(1.0 / 3.0, 1.0 - (2.0 * delta - 1.0) * (2.0 * delta - 1.0));
            beta = 0.0;
            for (int i = 0; i < PARAM; i++) beta += grad[i] * (grad[i] - prev_grad[i]) / sigma;

            for (int i = 0; i < PARAM; i++) {
                d[i] = -grad[i] + beta * d[i];
                prev_grad[i] = grad[i];
            }
        }
        else {
            lambda *= 2.0;
            for (int i = 0; i < PARAM; i++) params[i][0] -= phi * d[i];
        }
    }
}

// L-BFGS 最適化
void lbfgs_optimize(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1]) {
    double s[M][PARAM], y[M][PARAM], rho[M], q[PARAM], alpha[M], beta;
    double grad[PARAM], prev_grad[PARAM], prev_params[PARAM][1];

    int iter = 0, k = 0;
    //Compute_gradient(train_x, train_y, params, grad);

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

        //Compute_gradient(train_x, train_y, params, grad);

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