/* Prototype decreation */
/* Function for Matrix Product */
double InnerProduct(double[1][TRAIN_SIZE], double[TRAIN_SIZE][1]);
void InnerProduct_inv(double[TRAIN_SIZE][1], double[1][TRAIN_SIZE], double[TRAIN_SIZE][TRAIN_SIZE]);
void MatrixVector(double[TRAIN_SIZE][TRAIN_SIZE], double[TRAIN_SIZE][1], double[TRAIN_SIZE][1]);
void MatrixMatrix(double[TRAIN_SIZE][TRAIN_SIZE], double[TRAIN_SIZE][TRAIN_SIZE], double[TRAIN_SIZE][TRAIN_SIZE]);
double xTAx(double x[TRAIN_SIZE][1], double A[TRAIN_SIZE][TRAIN_SIZE]);
void Transpose(double[TRAIN_SIZE][1], double[1][TRAIN_SIZE]);
double Trace(double[TRAIN_SIZE][TRAIN_SIZE]);
void Scale(double[TRAIN_SIZE][TRAIN_SIZE], double);
/* Inverse */
double Determinant(double[TRAIN_SIZE][TRAIN_SIZE]);


/* Function for Matrix Product */
/* x'x */
double InnerProduct(double m1[1][TRAIN_SIZE], double m2[TRAIN_SIZE][1]) {
    double result[1][1];

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            result[i][j] = 0;
            for (int k = 0; k < TRAIN_SIZE; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result[0][0];
}

/* xx' */
void InnerProduct_inv(double m1[TRAIN_SIZE][1], double m2[1][TRAIN_SIZE], double result[TRAIN_SIZE][TRAIN_SIZE]) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < TRAIN_SIZE; j++) {
            for (int k = 0; k < 1; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/* Ax */ // 0215 better
void MatrixVector(double m1[TRAIN_SIZE][TRAIN_SIZE], double m2[TRAIN_SIZE][1], double result[TRAIN_SIZE][1]) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < 1; j++) {
            result[i][0] += m1[i][j] * m2[j][0];
        }
    }
}

/* A*A */
void MatrixMatrix(double m1[TRAIN_SIZE][TRAIN_SIZE], double m2[TRAIN_SIZE][TRAIN_SIZE], double result[TRAIN_SIZE][TRAIN_SIZE]) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < TRAIN_SIZE; j++) {
            for (int k = 0; k < TRAIN_SIZE; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/* x'Ax */
double xTAx(double x[TRAIN_SIZE][1], double A[TRAIN_SIZE][TRAIN_SIZE]) {
    double Ax[TRAIN_SIZE][1] = { 0.0 };
    double xT[1][TRAIN_SIZE] = { 0.0 };
    
    Transpose(x, xT);
    //printf("\nxT=\n");
    //for (int i = 0; i < TRAIN_SIZE; i++) {
    //    printf("%.2lf, ", xT[0][i]);
    //}

    MatrixVector(A, x, Ax);
    //printf("\nAx=\n");
    //for (int i = 0; i < TRAIN_SIZE; i++) {
    //    printf("%.2lf, ", Ax[i][0]);
    //}
    return InnerProduct(xT, Ax);
    //printf("Ax[4][0]=%.2lf\n", Ax[4][0]);
}

/* Transpose */
void Transpose(double x[TRAIN_SIZE][1], double xT[1][TRAIN_SIZE]) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
        xT[0][i] = x[i][0];
    }
}

/* Trace */
double Trace(double matrix[TRAIN_SIZE][TRAIN_SIZE]) {
    double tr = 0.0;
    for (int i = 0; i < TRAIN_SIZE; i++) {
        tr += matrix[i][i];
    }
    return tr;
}

/* Scale */
void Scale(double matrix[TRAIN_SIZE][TRAIN_SIZE], double scale) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < TRAIN_SIZE; j++) {
            matrix[i][j] *= scale;
        }
    }
}

// 内積計算
double dot_product(double* a, double* b, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

// ベクトルのスカラー倍
void scalar_mult(double* vec, double scalar, double* result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = vec[i] * scalar;
    }
}

// ベクトルの加算
void vector_add(double* a, double* b, double* result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = a[i] + b[i];
    }
}


// 行列の積
void mat_mult(double* A, double* B, double* C, int A_rows, int A_cols, int B_cols) {
    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            C[i * B_cols + j] = 0.0;
            for (int k = 0; k < A_cols; k++) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}


// 逆行列を求める（ガウス・ジョルダン法）
int inverse_matrix(double matrix[TRAIN_SIZE][TRAIN_SIZE], double inverse[TRAIN_SIZE][TRAIN_SIZE]) {
    static double temp = 0.0;

    // 拡張行列の作成（右側に単位行列を追加）
    double augmented[TRAIN_SIZE][2 * TRAIN_SIZE];
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < TRAIN_SIZE; j++) {
            augmented[i][j] = matrix[i][j]; // 元の行列
        }
        for (int j = TRAIN_SIZE; j < 2 * TRAIN_SIZE; j++) {
            augmented[i][j] = (i == j - TRAIN_SIZE) ? 1.0 : 0.0; // 単位行列
        }
    }

    // ガウス・ジョルダン法の適用
    for (int i = 0; i < TRAIN_SIZE; i++) {
        // ピボットの選択（対角成分が0の場合に行を入れ替える）
        if (fabs(augmented[i][i]) < 1e-9) {
            int swap_row = -1;
            for (int k = i + 1; k < TRAIN_SIZE; k++) {
                if (fabs(augmented[k][i]) > 1e-9) {
                    swap_row = k;
                    break;
                }
            }
            if (swap_row == -1) {
                return 0; // 逆行列なし（行列が特異）
            }
            // 行を交換
            for (int j = 0; j < 2 * TRAIN_SIZE; j++) {
                double temp = augmented[i][j];
                augmented[i][j] = augmented[swap_row][j];
                augmented[swap_row][j] = temp;
            }
        }

        // 対角成分を1にする
        temp = augmented[i][i];
        for (int j = 0; j < 2 * TRAIN_SIZE; j++) {
            augmented[i][j] /= temp;
        }

        // 他の行の該当列を0にする
        for (int k = 0; k < TRAIN_SIZE; k++) {
            if (k == i) continue;
            temp = augmented[k][i];
            for (int j = 0; j < 2 * TRAIN_SIZE; j++) {
                augmented[k][j] -= augmented[i][j] * temp;
            }
        }
    }

    // 逆行列を取得
    for (int i = 0; i < TRAIN_SIZE; i++) {
        for (int j = 0; j < TRAIN_SIZE; j++) {
            inverse[i][j] = augmented[i][j + TRAIN_SIZE];
        }
    }

    return 1; // 逆行列が求められた
}

// LU分解を行い、行列式を計算する
double Determinant(double matrix[TRAIN_SIZE][TRAIN_SIZE]) {
    double U[TRAIN_SIZE][TRAIN_SIZE];
    int i, j, k;
    int swap_count = 0;  // 行の交換回数（行列式の符号を決定）
    
    // U を初期化（元の行列をコピー）
    for (i = 0; i < TRAIN_SIZE; i++) {
        for (j = 0; j < TRAIN_SIZE; j++) {
            U[i][j] = matrix[i][j];
        }
    }

    // LU分解（ガウス消去法）
    for (i = 0; i < TRAIN_SIZE - 1; i++) {
        // ピボット選択（0回避）
        if (fabs(U[i][i]) < 1e-9) {
            int swap_row = -1;
            for (k = i + 1; k < TRAIN_SIZE; k++) {
                if (fabs(U[k][i]) > 1e-9) {
                    swap_row = k;
                    break;
                }
            }
            if (swap_row == -1) {
                return 0.0; // 逆行列が存在しない（行列式 = 0）
            }
            // 行の交換
            for (j = 0; j < TRAIN_SIZE; j++) {
                double temp = U[i][j];
                U[i][j] = U[swap_row][j];
                U[swap_row][j] = temp;
            }
            swap_count++;  // 行を交換した回数を記録
        }

        // 前進消去
        for (j = i + 1; j < TRAIN_SIZE; j++) {
            double factor = U[j][i] / U[i][i];
            for (k = i; k < TRAIN_SIZE; k++) {
                U[j][k] -= factor * U[i][k];
            }
        }
    }

    // 行列式は U の対角成分の積
    double det = 1.0;
    for (i = 0; i < TRAIN_SIZE; i++) {
        det *= U[i][i];
    }

    // 行交換が奇数回なら符号を反転
    return (swap_count % 2 == 0) ? det : -det;
}
