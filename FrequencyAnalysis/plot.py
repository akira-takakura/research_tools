# output to figure

import pandas as pd
import math
import numpy as np
from scipy.spatial import distance
import matplotlib.pyplot as plt
from scipy.signal import max_len_seq



def main():
    CURRENT_PATH = "data/"
    OUT_PATH = "fig_final/"
    
    print("Start! Drawing")
    plt.rcParams['font.family'] = 'Times New Roman'
    plt.rcParams['font.size'] = 20
    plt.rcParams['xtick.direction'] = 'in'#x軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')
    plt.rcParams['ytick.direction'] = 'in'#y軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')

    ### data revise ###
    CPR_DATA_NAME = "bilateral_data.csv"
    STD_DATA_NAME = "std_data.csv"
    

    # df = pd.read_csv(CURRENT_PATH + CPR_DATA_NAME, header = 0)
    # df_std = pd.read_csv(CURRENT_PATH + STD_DATA_NAME, header = 0)
    
    # FFT次
    df = pd.read_csv('data/bilateral_data_FFT_6.5_7.5.csv', header = 0)
    df_std = pd.read_csv('data/std_data_FFT_6.5_7.5.csv', header = 0)
    # df_adapt = pd.read_csv(CURRENT_PATH + "adapt.csv", header = 0)
    
    ### plot ###
    # FFT
    plt.plot(df_std["freq"], df_std[" amp"], color = "red", linestyle = 'solid', label = "conv")
    plt.plot(df["freq"], df[" amp"], color = "blue", linestyle = 'solid', label = "prop")
    plt.rcParams['text.usetex'] = True
    plt.xscale("log")
    plt.xlim(1,100)
    plt.xlabel('frequency [Hz]')
    plt.ylabel(r'Amplitude [N]')
    plt.legend()
    
    plt.tight_layout()
    plt.savefig(OUT_PATH + "std_bilate_result.png")
    plt.show()
    plt.clf()
    plt.close()


    ## bilateral result
    row_number = 2
    column_number = 1
    # figure()でグラフを表示する領域をつくり，figというオブジェクトにする．
    fig = plt.figure(figsize=(7 * row_number, 5 * column_number))

    # add_subplot()でグラフを描画する領域を追加する．引数は行，列，場所
    ax = [0]*(row_number*column_number)
    for i in range(row_number*column_number):
      ax[i] = fig.add_subplot(column_number, row_number, i+1)

    # ax[0].plot(df_std["t"], df_std[" leader_x"], color = "dodgerblue", linestyle = 'solid', label = "Leader")
    # ax[0].plot(df["t"], df_std[" follower_x"], color = "orange", linestyle = 'dashed', label = "Follower")
    # ax[0].set_xlabel('Time [s]')
    # ax[0].set_ylabel('Position [m]')
    # ax[0].legend()

    # ax[1].plot(df_std["t"], df_std[" leader_f_dob"], color = "dodgerblue", linestyle = 'solid', label = "Leader")
    # ax[1].plot(df["t"], df_std[" follower_f_dob"], color = "orange", linestyle = 'dashed', label = "Follower")
    # ax[1].set_xlabel('Time [s]')
    # ax[1].set_ylabel('Force [N]')
    # ax[1].set_xlim(-0.3, 11.1)
    # ax[1].set_ylim(-85, 85)
    # ax[1].legend()
    
    # plt.tight_layout()
    # plt.savefig(OUT_PATH + "std_bilate_result.png")
    # plt.show()
    # plt.clf()
    # plt.close()

    # ## 4ch bilateral結果
    # row_number = 2
    # column_number = 1
    # # figure()でグラフを表示する領域をつくり，figというオブジェクトにする．
    # fig = plt.figure(figsize=(7 * row_number, 5 * column_number))

    # # add_subplot()でグラフを描画する領域を追加する．引数は行，列，場所
    # ax = [0]*(row_number*column_number)
    # for i in range(row_number*column_number):
    #   ax[i] = fig.add_subplot(column_number, row_number, i+1)
    
    # ax[0].plot(df["t"], df[" leader_x"], color = "dodgerblue", linestyle = 'solid', label = "Leader")
    # ax[0].plot(df["t"], df[" follower_x"], color = "orange", linestyle = 'dashed', label = "Follower")
    # ax[0].set_xlabel('Time [s]')
    # ax[0].set_ylabel('Position [m]')
    # ax[0].legend()

    # ax[1].plot(df["t"], df[" leader_f_dob"], color = "dodgerblue", linestyle = 'solid', label = "Leader")
    # ax[1].plot(df["t"], df[" follower_f_dob"], color = "orange", linestyle = 'dashed', label = "Follower")
    # ax[1].set_xlabel('Time [s]')
    # ax[1].set_ylabel('Force [N]')
    # ax[1].set_xlim(-0.3, 11.1)
    # ax[1].set_ylim(-85, 85)
    # ax[1].legend()
    
    # plt.tight_layout()
    # plt.savefig(OUT_PATH + "prop_bilate_result.png")
    # plt.show()
    # plt.clf()
    # plt.close()
    
    # ## identificayion
    # row_number = 2
    # column_number = 1
    # # figure()でグラフを表示する領域をつくり，figというオブジェクトにする．
    # fig = plt.figure(figsize=(7 * row_number, 5 * column_number))

    # # add_subplot()でグラフを描画する領域を追加する．引数は行，列，場所
    # ax = [0]*(row_number*column_number)
    # for i in range(row_number*column_number):
    #   ax[i] = fig.add_subplot(column_number, row_number, i+1)
    
    # ax[0].plot(df_std["t"], df_std[" Kenv_ideal"], color = "dodgerblue", linestyle = 'solid', label = "Real")
    # ax[0].plot(df_std["t"], df_std[" Kenv_hat[10]"], color = "orange", linestyle = 'dashed', label = "Estimated")
    # ax[0].set_xlabel('Time [s]')
    # ax[0].set_ylabel('Stiffness [N/m]')
    # ax[0].set_ylim(-50, 1300)
    # ax[0].legend()

    # ax[1].plot(df["t"], df_std[" Denv_ideal"], color = "dodgerblue", linestyle = 'solid', label = "Real")
    # ax[1].plot(df["t"], df_std[" Denv_hat"], color = "orange", linestyle = 'dashed', label = "Estimated")
    # ax[1].set_xlabel('Time [s]')
    # ax[1].set_ylabel('Viscosity [Ns/m]')
    # ax[1].set_ylim(-3, 85)
    # ax[1].legend()
    # plt.savefig(OUT_PATH + "conv_parameters.png")
    # plt.tight_layout()
    
    # plt.show()
    # plt.clf()
    # plt.close()
    
    # row_number = 2
    # column_number = 1
    # # figure()でグラフを表示する領域をつくり，figというオブジェクトにする．
    # fig = plt.figure(figsize=(7 * row_number, 5 * column_number))

    # # add_subplot()でグラフを描画する領域を追加する．引数は行，列，場所
    # ax = [0]*(row_number*column_number)
    # for i in range(row_number*column_number):
    #   ax[i] = fig.add_subplot(column_number, row_number, i+1)
    
    # ax[0].plot(df["t"], df[" Kenv_ideal[20]"], color = "dodgerblue", linestyle = 'solid', label = "Real")
    # ax[0].plot(df["t"], df[" Kenv_hat"], color = "orange", linestyle = 'dashed', label = "Estimated")
    # ax[0].set_xlabel('Time [s]')
    # ax[0].set_ylabel('Stiffness [N/m]')
    # ax[0].set_ylim(-50, 1300)
    # ax[0].legend()

    # ax[1].plot(df["t"], df[" Denv_ideal"], color = "dodgerblue", linestyle = 'solid', label = "Real")
    # ax[1].plot(df["t"], df[" Denv_hat"], color = "orange", linestyle = 'dashed', label = "Estimated")
    # ax[1].set_xlabel('Time [s]')
    # ax[1].set_ylabel('Viscosity [Ns/m]')
    # ax[1].set_ylim(-3, 85)
    # ax[1].legend()
    
    # plt.tight_layout()
    # plt.savefig(OUT_PATH + "prop_parameters.png")
    # plt.show()
    # plt.clf()
    # plt.close()


    # ## convとpropの比較
    # # step2 グラフフレームの作成
    # ## Force ###
    # fig, ax = plt.subplots(figsize=[6.4, 4.8])
    # # step3 折れ線グラフの描画
    # ax.plot(df_std["t"], df_std[" leader_f_dob"], color = "red", linestyle = 'solid', label = "conv")
    # ax.plot(df["t"], df[" leader_f_dob"], color = "dodgerblue", linestyle = 'solid', label = "prop") 
    # ax.set_xlabel('Time [s]')
    # ax.set_ylabel('Force [N]')
    # ax.legend(loc='lower left') # 凡例位置制御
    # plt.tight_layout()

    # # # step4 zoomしたグラフの描画
    # # axins = ax.inset_axes([0.4, 0.4, 0.5, 0.5])
    # # axins.plot(df_std["t"], df_std[" leader_f_dob"], color = "red", linestyle = 'solid', label = "conv")
    # # axins.plot(df["t"], df[" leader_f_dob"], color = "dodgerblue", linestyle = 'solid', label = "prop")
    # # # zoomしたグラフの設定
    # # x1, x2, y1, y2 = 2.5, 3.5, -75, -60
    # # axins.set_xlim(x1, x2)
    # # axins.set_ylim(y1, y2)
    # # ax.indicate_inset_zoom(axins)
    # plt.savefig(OUT_PATH + "f_compare.png")
    # plt.show()
    # plt.clf()
    # plt.close()

    # ### velocity ###
    # fig, ax = plt.subplots(figsize=[6.4, 4.8])
    # # step3 折れ線グラフの描画
    # ax.plot(df_std["t"], df_std[" leader_dx"], color = "red", linestyle = 'solid', label = "conv")
    # ax.plot(df["t"], df[" leader_dx"], color = "dodgerblue", linestyle = 'solid', label = "prop")
    # ax.set_xlabel('Time [s]')
    # ax.set_ylabel('Velocity [m/s]')
    # ax.legend(loc='lower left') # 凡例位置制御
    # plt.tight_layout()

    # # # step4 zoomしたグラフの描画
    # # axins = ax.inset_axes([0.4, 0.4, 0.5, 0.5])
    # # axins.plot(df_std["t"], df_std[" leader_dx"], color = "red", linestyle = 'solid', label = "conv")
    # # axins.plot(df["t"], df[" leader_dx"], color = "dodgerblue", linestyle = 'solid', label = "prop")
    # # # zoomしたグラフの設定
    # # x1, x2, y1, y2 = 3, 4, -0.15, 0.15
    # # axins.set_xlim(x1, x2)
    # # axins.set_ylim(y1, y2)
    # # ax.indicate_inset_zoom(axins)
    
    # plt.savefig(OUT_PATH + "dx_compare.png")
    # plt.show()
    # plt.clf()
    # plt.close()

    # ### position ###
    # fig, ax = plt.subplots(figsize=[6.4, 4.8])
    # # step3 折れ線グラフの描画
    # ax.plot(df_std["t"], df_std[" leader_x"], color = "red", linestyle = 'solid', label = "conv")
    # ax.plot(df["t"], df[" leader_x"], color = "dodgerblue", linestyle = 'solid', label = "prop") 
    # ax.set_xlabel('Time [s]')
    # ax.set_ylabel('Position [m]')
    # ax.legend(loc='upper left') # 凡例位置制御
    # plt.tight_layout()

    # # step4 zoomしたグラフの描画
    # axins = ax.inset_axes([0.4, 0.1, 0.5, 0.5])
    # axins.plot(df_std["t"], df_std[" leader_x"], color = "red", linestyle = 'solid', label = "conv")
    # axins.plot(df["t"], df[" leader_x"], color = "dodgerblue", linestyle = 'solid', label = "prop")
    # # zoomしたグラフの設定
    # x1, x2, y1, y2 = 6.75, 7.35, 1.050, 1.080
    # axins.set_xlim(x1, x2)
    # axins.set_ylim(y1, y2)
    # ax.indicate_inset_zoom(axins)
    # plt.savefig(OUT_PATH + "x_compare.png")
    
    # plt.show()
    # plt.clf()
    # plt.close()

    ## convとpropの比較###################################################(gap)
    # step2 グラフフレームの作成
    ## Force ###
    # fig, ax = plt.subplots(figsize=[6.4, 4.8])
    # # step3 折れ線グラフの描画
    # ax.plot(df_std["t"], df_std[" leader_f_dob"] - df[" leader_f_dob"], color = "red", linestyle = 'solid')
    # # ax.plot(df["t"], df[" leader_f_dob"], color = "dodgerblue", linestyle = 'solid', label = "prop") 
    # ax.set_xlabel('Time [s]')
    # ax.set_ylabel('Force gap[N]')
    # plt.tight_layout()
    
    # plt.savefig(OUT_PATH + "f_compare_gap.png")
    # plt.show()
    # plt.clf()
    # plt.close()

    # ### velocity ###
    # fig, ax = plt.subplots(figsize=[6.4, 4.8])
    # # step3 折れ線グラフの描画
    # ax.plot(df_std["t"], df_std[" leader_dx"] - df[" leader_dx"], color = "red", linestyle = 'solid', label = "conv")
    # # ax.plot(df["t"], df[" leader_dx"], color = "dodgerblue", linestyle = 'solid', label = "prop")
    # ax.set_xlabel('Time [s]')
    # ax.set_ylabel('Velocity [m/s]')
    # ax.legend(loc='lower left') # 凡例位置制御
    # plt.tight_layout()
    
    # plt.savefig(OUT_PATH + "dx_compare_gap.png")
    # plt.show()
    # plt.clf()
    # plt.close()

    # ### position ###
    # fig, ax = plt.subplots(figsize=[6.4, 4.8])
    # # step3 折れ線グラフの描画
    # ax.plot(df_std["t"], df_std[" leader_x"] - df[" leader_x"], color = "red", linestyle = 'solid', label = "conv")
    # # ax.plot(df["t"], df[" leader_x"], color = "dodgerblue", linestyle = 'solid', label = "prop") 
    # ax.set_xlabel('Time [s]')
    # ax.set_ylabel('Position [m]')
    # ax.legend(loc='upper left') # 凡例位置制御
    # plt.tight_layout()
    
    # plt.show()
    # plt.clf()
    # plt.close()

    ## compare
    # plt.plot(df["t"], df[" leader_f_dob"], color = "blue", linestyle = 'solid', label = "prop")
    # plt.xlabel("Time [s]")
    # plt.xlabel("")
    
    # plt.show()
    # plt.savefig()
    # plt.clf()
    # plt.close()

    
    ### if multiplot ###
    # row_number = 2
    # column_number = 3
    # figure()でグラフを表示する領域をつくり，figというオブジェクトにする．
    # fig = plt.figure(figsize=(7 * row_number, 5 * column_number))

    # # add_subplot()でグラフを描画する領域を追加する．引数は行，列，場所
    # ax = [0]*(row_number*column_number)
    # for i in range(row_number*column_number):
    #   ax[i] = fig.add_subplot(column_number, row_number, i+1)
    
    # ax[0].plot(df_std["t"], df_std[" leader_x"], color = "red", linestyle = 'solid', label = "conv")
    # ax[0].plot(df["t"], df[" leader_x"], color = "blue", linestyle = 'solid', label = "prop")
    # ax[0].set_ylim(1.95, 2.05)
    # ax[0].set_xlabel('Time [s]')
    # ax[0].set_ylabel('leader posoition [m]')
    # ax[0].legend()

    # ax[1].plot(df_std["t"], df_std[" leader_x"] - df[" leader_x"], color = "red", linestyle = 'solid', label = "conv-prop")
    # ax[1].set_xlabel('Time [s]')
    # ax[1].set_ylabel('leader posoition [m]')
    # ax[1].legend()

    # ax[2].plot(df_std["t"], df_std[" leader_dx"], color = "red", linestyle = 'solid', label = "conv")
    # ax[2].plot(df["t"], df[" leader_dx"], color = "blue", linestyle = 'solid', label = "prop")
    # ax[2].set_xlabel('Time [s]')
    # ax[2].set_ylabel('leader velocity [m/s]')
    # ax[2].legend()
    
    # ax[3].plot(df["t"], df_std[" leader_dx"] - df[" leader_dx"], color = "red", linestyle = 'solid', label = "conv-prop")
    # ax[3].set_xlabel('Time [s]')
    # ax[3].set_ylabel('leader velocity [m/s]')
    # ax[3].legend()

    # ax[4].plot(df_std["t"], df_std[" leader_f_dob"], color = "red", linestyle = 'solid', label = "conv")
    # ax[4].plot(df["t"], df[" leader_f_dob"], color = "blue", linestyle = 'solid', label = "prop")
    # ax[4].set_xlabel('Time [s]')
    # ax[4].set_ylabel('leader F_DOB [N]')
    # ax[4].legend()
    
    # ax[5].plot(df["t"], df_std[" leader_f_dob"] - df[" leader_f_dob"], color = "red", linestyle = 'solid', label = "conv-prop")
    # ax[5].set_xlabel('Time [s]')
    # ax[5].set_ylabel('leader F_DOB [N]')
    # ax[5].legend()
    


    #### if expand partially 
    # import matplotlib.pyplot as plt
    # import numpy as np

    # # step1 データの作成
    # x = np.linspace(0, 10, 100)
    # y1 = 4 + 2 * np.sin(2 * x)
    # y2 = 4 + 2 * np.cos(2 * x)
    # # step2 グラフフレームの作成
    # fig, ax = plt.subplots(figsize=[6.4, 4.8])
    # # step3 折れ線グラフの描画
    # ax.plot(x, y1, linestyle='-', label='Sample 1')
    # ax.plot(x, y2, linestyle='--', label='Sample 2')

    # ax.set_xlim(0, 10)
    # ax.set_ylim(0, 10)
    # ax.set_xlabel('X label')
    # ax.set_ylabel('Y label')
    # ax.legend(loc='upper left')

    # # step4 zoomしたグラフの描画
    # axins = ax.inset_axes([0.6, 0.6, 0.37, 0.37])
    # axins.plot(x, y1, linestyle='-', label='Sample 1')
    # axins.plot(x, y2, linestyle='--', label='Sample 2')
    # # zoomしたグラフの設定
    # x1, x2, y1, y2 = 2, 4, 4, 6
    # axins.set_xlim(x1, x2)
    # axins.set_ylim(y1, y2)
    # ax.indicate_inset_zoom(axins)

    # plt.show()

if __name__ == "__main__":
    main()
    
