import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.linear_model import LassoCV
from sklearn.linear_model import Lasso
from sklearn.metrics import mean_squared_error
import os
import sys
import math

def Obtain_Current_Directory():
    try:
        current_dir = os.path.dirname(os.path.abspath(__file__))
    except NameError:  # __file__ が定義されていない場合
        current_dir = os.getcwd()  # カレントディレクトリを取得
    current_dir += "\\" 
    return current_dir

# 本文
def main():
    # パラメータ設定
    learning_rate = 0.01  # 学習率
    # lambda_ = 0.1         # 正則化係数
    # iterations = 1000     # 反復回数
    # datanum = 1000        # データ点の数
    # dim = 3               # 説明変数の次元
    
    CURRENT_DIR = Obtain_Current_Directory()

    # データ格納
    df = pd.read_csv(CURRENT_DIR + 'train.csv', header = 0)
    # df = pd.read_csv(CURRENT_DIR + 'test.csv', header = 0)

    t = np.array(df["t"])
    x = np.array(df["x"])
    dx = np.array(df["dx"])
    ddx = np.array(df["ddx"])
    x2 = np.array(df["x2"])
    dx2 = np.array(df["dx2"])
    xdx = np.array(df["xdx"])
    c = np.array(df["c"])
    f = np.array(df["f"])
    
    # print(t)

    # データ構成
    X_train = [x, dx, ddx, c, x2, xdx, dx2]
    X_train = np.array(X_train).T
    y_train = f
    # print(X_train)

    # https://helve-blog.com/posts/python/sklearn-lasso-regression/
    reg = Lasso(alpha=0.00000010)
    # 学習
    reg.fit(X_train, y_train)
    # 結果表示
    print(reg.coef_)
    print(reg.intercept_)
    
    # # # https://datawokagaku.com/lasso/
    # mse_list = []
    # coefs = []
    # labels= ["Stiffness (K)", "Stiffness (D)", "Mass (M)", "Constant (C)", "(K2)", "(KD)", "(D2)"]
    
    # # #case1
    # # from_alpha = 0.0010
    # # to_alpha = 0.10
    # #case2
    # from_alpha = 0.000000010
    # to_alpha = 0.550
    
    # # alphas = np.linspace(0.000000010,0.550,100)   #1
    # alphas = np.linspace(from_alpha, to_alpha, 1000)     #2
    # print("alphas=")
    # print(alphas)
    # print("\n")
    # for alpha in alphas:
    #     model = Lasso(alpha=alpha)
    #     model.fit(X_train, y_train)
    #     # y_pred = model.predict(X_test)
    #     # mse = mean_squared_error(y_test, y_pred)
    #     # mse_list.append(mse)
    #     coefs.append(model.coef_)
    #     # print("model_coef=")
    #     # print(model.coef_)
    #     # plt.plot(alphas[i], coefs, label=labels[i])
    
    # coefs = np.array(coefs).T

    # # 図示
    # print("Start! Drawing")
    # plt.rcParams['font.family'] = 'Times New Roman'
    # plt.rcParams['font.size'] = 20
    # plt.rcParams['xtick.direction'] = 'in'#x軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')
    # plt.rcParams['ytick.direction'] = 'in'#y軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')

    # for i in range (7):     # 
    #     # plt.scatter(alphas, coefs[i], label=labels[i])
    #     plt.plot(alphas, coefs[i], label=labels[i])
    # plt.rcParams['text.usetex'] = True
    # plt.xscale("log")
    # plt.xlim(from_alpha,to_alpha)
    # plt.ylim(-1,1)
    # plt.xlabel('lambda ')
    # plt.ylabel('standardized coefficients')
    # # plt.tight_layout()
    # plt.legend()
    # plt.savefig("-8-0_1000.png")
    # # plt.savefig("-3-1_1000.png")
    # plt.show()
    # plt.clf()
    # plt.close()

    # https://qiita.com/suzuki0430/items/e3958f027af1dd011f21
    # 1~100まで0.1刻みでαをかえる
    # reg = LassoCV(eps=0.0001,n_alphas=1,cv=5) ## alpha = 0.550で全パラ0
    # reg.fit(X_train,y_train)

    # テストデータの予測
    # test_predictions = lasso_cv_model.predict(X_test)
    
    # # 結果表示
    # print(reg.coef_)
    # print(reg.intercept_)


# メイン処理
if __name__ == "__main__":
    main()
    


# # ラッソ回帰の実装
# def lasso_regression(X, Y):
#     # パラメータの初期化
#     Theta = np.zeros((dim + 1, 1))  # バイアス項を含む

#     # 学習のループ
#     for iter in range(iterations):
#         gradients = np.zeros_like(Theta)

#         # 勾配の計算
#         for i in range(datanum):
#             x_i = np.insert(X[i], 0, 1)  # バイアス項を追加
#             error = np.dot(Theta.T, x_i) - Y[i]
#             gradients += error * x_i.reshape(-1, 1)

#         # 正則化項を追加
#         for j in range(1, dim + 1):
#             if Theta[j] > 0:
#                 gradients[j] += lambda_
#             elif Theta[j] < 0:
#                 gradients[j] -= lambda_

#         # パラメータの更新
#         Theta -= (learning_rate / datanum) * gradients

#         # 進捗の表示（オプション）
#         if iter % 100 == 0:
#             cost = compute_cost(X, Y, Theta)
#             print(f"Iteration {iter}: Cost = {cost}")

#     return Theta

# # コスト関数の計算
# def compute_cost(X, Y, Theta):
#     total_cost = 0
#     for i in range(datanum):
#         x_i = np.insert(X[i], 0, 1)  # バイアス項を追加
#         prediction = np.dot(Theta.T, x_i)
#         error = prediction - Y[i]
#         total_cost += error**2
#     # 正則化項を追加
#     total_cost += lambda_ * np.sum(np.abs(Theta[1:]))
#     return total_cost / (2 * datanum)

# # メイン処理
# if __name__ == "__main__":
#     # データセットの作成（例としてランダムデータを生成）
#     np.random.seed(42)  # 再現性のためのシード設定
#     X = np.random.rand(datanum, dim) * 10  # 説明変数
#     true_Theta = np.array([[5], [3], [2], [-1]])  # 真のパラメータ
#     Y = X @ true_Theta[1:] + true_Theta[0] + np.random.randn(datanum, 1) * 0.5  # 目的変数

#     # ラッソ回帰の実行
#     estimated_Theta = lasso_regression(X, Y)

#     # 推定結果の表示
#     print("\nEstimated Parameters:")
#     for i, theta in enumerate(estimated_Theta):
#         print(f"Theta[{i}] = {theta[0]:.4f}")
       