/* プロトタイプ宣言 */
// ガウス過程回帰
double Delta(double, double);
double Delta_int(int, int);
double Kernel_function(double, double, double[PARAM][1]);
void Compute_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double K[TRAIN_SIZE][TRAIN_SIZE]);
void Compute_partial_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double K[TRAIN_SIZE][TRAIN_SIZE], int);
void GPR(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[N][1], double[N][1], double[1][1]);
// ハイパーパラメータの最適化
double Compute_gradient(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[PARAM][1], int);

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
    double K_inv[TRAIN_SIZE][TRAIN_SIZE] = { 0.0 };       // 逆行列
    double yTKy = 0.0;

    Compute_kernel_matrix(train_x, params, K);  // カーネル行列を算出
    det = Determinant(K);
    inverse_matrix(K, K_inv);

    yTKy = xTAx(train_y, K_inv);      // y'* alpha

    return  -log(det) - yTKy;
    //return -0.50 * TRAIN_SIZE * log(2.0 * PI) - 0.50 * log(det) - 0.50 * yTKy;
}


// カーネル行列の偏微分を計算
void Compute_partial_kernel_matrix(double train_x[TRAIN_SIZE][1], double params[PARAM][1], double part_K[TRAIN_SIZE][TRAIN_SIZE], int param) {
    // d_tau
    if (param == 0) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            for (int j = 0; j < TRAIN_SIZE; j++) {
                part_K[i][j] = 0.0; // 明示的に初期化
                part_K[i][j] = Kernel_function(train_x[i][0], train_x[j][0], params) - params[2][0] * Delta_int(i, j);
            }
        }
    }
    // d_sigma
    else if (param == 1) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            for (int j = 0; j < TRAIN_SIZE; j++) {
                part_K[i][j] = 0.0; // 明示的に初期化
                part_K[i][j] = Kernel_function(train_x[i][0], train_x[j][0], params) - params[2][0] * Delta_int(i, j);
                part_K[i][j] *= pow(train_x[i][0] - train_x[j][0], 2.0) / params[1][0];
            }
        }
    }
    // d_eta
    else if (param == 2) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            for (int j = 0; j < TRAIN_SIZE; j++) {
                part_K[i][j] = 0.0; // 明示的に初期化
                part_K[i][j] = params[2][0] * Delta_int(i, j);
            }
        }
    }
}


// 尤度関数の勾配を計算
/* https://qiita.com/meltyyyyy/items/5a058ecc81e010876a39 */
double Compute_gradient(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1], int param) {
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

    // 1st term
    MatrixMatrix(K_theta_inv, part_K_theta, in_trace);
    first_term = Trace(in_trace);

    // 2nd term
    MatrixVector(K_theta_inv, train_y, alpha);    // alpha = K^-1 * y
    second_term = xTAx(alpha, part_K_theta);
    
    return -1.0*first_term + second_term;
}
