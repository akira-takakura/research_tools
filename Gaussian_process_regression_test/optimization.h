/* 最適化アルゴリズム */

void GD(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[PARAM][1]);
void CGD(double[TRAIN_SIZE][1], double[TRAIN_SIZE][1], double[PARAM][1]);


// 最急降下法(GD)による最適化
void GD(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1], double params_grad[PARAM][1]) {
    // 変数変換 [tau, sigma, eta]
    double params_log[PARAM][1] = { log(params[0][0]), log(params[1][0]), log(params[2][0]) };

    for (int i = 0; i < PARAM; i++) {
        params_grad[i][0] = Compute_gradient(train_x, train_y, params, i);   // 各パラiに対して方向決定
        params_log[i][0] += Alpha * params_grad[i][0];                                 // パラメータ更新
        params[i][0] = pow(E, params_log[i][0]);
    }
}

// 共役勾配法（Conjugate Gradient Method, CG）によるハイパーパラメータ最適化
void CGD(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1], double params_grad[PARAM][1]) {
    double grad[PARAM][1], prev_grad[PARAM][1], direction[PARAM][1], step[PARAM][1] = { 0.0 };
    double beta, grad_norm, prev_grad_norm;

    // tau-sigma-eta
    for (int i = 0; i < PARAM; i++) {
        params_grad[i][0] = -1.0 * Compute_gradient(train_x, train_y, params, i);

        // 初期方向を勾配の負の方向に設定
        for (int i = 0; i < PARAM; i++) {
            direction[i][0] = -1.0 * params_grad[i][0];
        }

        for (int iter = 0; iter < MAX_ITER; iter++) {
            grad_norm = dot_product(params_grad, params_grad);

            //// 収束条件: ||∇L(θ)|| < EPSILON
            //if (sqrt(grad_norm) < EPSILON) {
            //    printf("収束しました。反復回数: %d\n", iter);
            //    break;
            //}

            // ステップ長 α を計算
            //alpha = Alpha;  // 固定の学習率（ラインサーチを追加することも可能）

            // 方向に沿って θ を更新
            scalar_mult(direction, Alpha, step);   // stepを求める
            vector_add(params, step, params);

            // 新しい勾配を計算
            prev_grad[i][0] = -1.0 * params_grad[i][0];

            params_grad[i][0] = Compute_gradient(train_x, train_y, params, i);

            // 共役係数 β を計算（Fletcher-Reeves法）
            prev_grad_norm = grad_norm;
            grad_norm = dot_product(params_grad, params_grad);
            beta = grad_norm / prev_grad_norm;

            // 新しい探索方向を更新
            scalar_mult(direction, beta, step);
            for (int i = 0; i < PARAM; i++) {
                direction[i][0] = -1.0 * params_grad[i][0] + step[i][0];
            }
            printf("Iteration %d: Theta=[%lf, %lf, %lf], L=%lf\n", iter, params[0][0], params[1][0], params[2][0], compute_log_likelihood(train_x, train_y, params));
        }
    }
}

// スケーリング共役勾配法（Scaled Conjugate Gradient Descent, SCGD）によるハイパーパラメータ最適化
void SCGD(double train_x[TRAIN_SIZE][1], double train_y[TRAIN_SIZE][1], double params[PARAM][1], double params_grad[PARAM][1]) {
    double grad[PARAM][1], prev_grad[PARAM][1], direction[PARAM][1], step[PARAM][1];
    double sigma = SIGMA, beta, grad_norm, prev_grad_norm, mu, phi, delta, new_L;
    double L = compute_log_likelihood(train_x, train_y, params);

    // 初期勾配を計算
    for (int i = 0; i < PARAM; i++) {
        grad[i][0] = Compute_gradient(train_x, train_y, params, i);
    }

    // 初期方向を勾配の負の方向に設定
    for (int i = 0; i < PARAM; i++) {
        direction[i][0] = -grad[i][0];
    }

    for (int iter = 0; iter < MAX_ITER; iter++) {
        grad_norm = dot_product(grad, grad);

        // 収束条件: ||∇L(θ)|| < EPSILON
        if (sqrt(grad_norm) < EPSILON) {
            printf("収束しました。反復回数: %d\n", iter);
            break;
        }

        // ステップ長 α を計算
        mu = grad_norm + Alpha * dot_product(direction, direction);
        phi = -grad_norm / mu;

        // 方向に沿って θ を更新
        scalar_mult(direction, phi, step);
        vector_add(params, step, params);

        // 新しい勾配を計算
        for (int i = 0; i < PARAM; i++) {
            prev_grad[i][0] = grad[i][0];
            grad[i][0] = Compute_gradient(train_x, train_y, params, i);
        }

        // 共役係数 β を計算（Fletcher-Reeves法）
        prev_grad_norm = grad_norm;
        grad_norm = dot_product(grad, grad);
        beta = grad_norm / prev_grad_norm;

        // 新しい探索方向を更新
        scalar_mult(direction, beta, step);
        for (int i = 0; i < PARAM; i++) {
            direction[i][0] = -grad[i][0] + step[i][0];
        }

        // ログ出力
        new_L = compute_log_likelihood(train_x, train_y, params);
        printf("Iteration %d: Theta=[%lf, %lf, %lf], L=%lf\n", iter, params[0][0], params[1][0], params[2][0], new_L);
    }
}