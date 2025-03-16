#define N			1		// データ点数 (10秒 / 0.01秒 = 1000)
#define STEP		10		// ステップ数
#define TRAIN_SIZE	5		// 訓練データのサイズ
#define NOISE_VAR	0.000		// ノイズ分散

/* ハイパーパラメータの学習 */
#define PARAM		3
#define EPSILON		1e-1	// 収束判定
#define MAX_ITER	1
#define Alpha		0.10	// 学習率
#define SIGMA 1e-4     // スケーリングパラメータ

#define ST	0.00010
#define PI	3.1416
#define E	2.7183