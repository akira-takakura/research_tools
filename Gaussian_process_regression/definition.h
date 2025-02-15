#define N			1		// データ点数 (10秒 / 0.01秒 = 1000)
#define TRAIN_SIZE	5		// 訓練データのサイズ
#define NOISE_VAR	1.00	// ノイズ分散

/* ハイパーパラメータの学習 */
#define PARAM		3
#define M			5		// 記憶する更新履歴の長さ
#define MAX_ITER	10		// 最大イテレーション数
#define EPSILON		1e-2	// 収束判定

// 力回帰用
#define theta1 1.0   // 
#define theta2 10.0   // = 2 * \sigma^2, (\sigmaが大きい場合、広い範囲のデータを類似とみなします。)
#define theta3 5.0    //    

//// 位置回帰用
//#define theta1 10.0     // 
//#define theta2 1.0      // 
//#define theta3 0.10     //  

#define ST 0.00010