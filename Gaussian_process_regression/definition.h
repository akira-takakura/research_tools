#define N			1		// データ点数 (10秒 / 0.01秒 = 1000)
#define TRAIN_SIZE	5		// 訓練データのサイズ
#define NOISE_VAR	0.0		// ノイズ分散

/* ハイパーパラメータの学習 */
#define PARAM		3
#define M			5		// 記憶する更新履歴の長さ
#define MAX_ITER	10		// 最大イテレーション数
#define EPSILON		1e-1	// 収束判定

#define ST	0.00010
#define PI	3.1416
#define E	2.7183