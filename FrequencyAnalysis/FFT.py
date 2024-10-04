from math import fabs
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# https://qiita.com/chnokrn/items/57c89382942778a20e7f

FILE = "bilateral_data"
# FILE = "std_data"
from_t = 6.5
to_t = 7.5
obj_list = [" leader_f_dob"]
t_list = [0,5,10]

class FOURIER:
    def __init__(self, samplingtime, dataframe, objective_name, output_path, start_time:float, end_time:float):
        self.t_from = start_time
        self.t_to = end_time
        self.N = 2 + int((self.t_to - self.t_from) * 10000)
        self.t_offset = start_time * 10000
        self.ST = samplingtime
        self.df = dataframe
        self.obj_name = objective_name
        
        self.output = open(output_path, 'w')
        self.output.write(f"freq, amp\n")
        print("FOURIER Initialized by data '{}'".format(output_path))

    def _Read_data(self):
        print("[Called!] Reading data in csv")
        print(self.t_offset)
        self.t = []
        self.obj = []
        
        for i in range(self.N):
            j = self.t_offset   # t=0以外からのデータが欲しい時
            self.t.append(df["t"][i + j])
            self.obj.append(df[self.obj_name][i + j])
        
        # print(type(self.obj))

        self.t_list = self.t
        self.obj_list = self.obj
        self.t = np.array(self.t)
        self.obj = np.array(self.obj)

        # print(type(self.obj))

        plt.plot(self.t_list, self.obj_list, color="blue", linestyle='solid')
        plt.xscale("linear")
        plt.title(self.obj_name + "'s original time data", y=-0.25)
        plt.xlabel('Time [s]')
        plt.ylabel(self.obj_name + '[N]')
        plt.savefig("fig/ORIGIN" + FILE + ".png", bbox_inches='tight')
        # plt.show()
        plt.clf()
        plt.close()
    
    def _FFT(self):
        # print("[Called!] FFT")
        obj_fft = np.fft.fft(self.obj)              # 離散フーリエ変換
        Freq = np.fft.fftfreq(self.N, d = self.ST)  # 周波数を割り当てる
        Amp = abs(obj_fft / (self.N / 2))           # 音の大きさ（振幅の大きさ）
        self.freq = Freq[1 : int(self.N / 2)]
        self.amp = Amp[1 : int(self.N / 2)]
        self.freq_list = list(self.freq)
        self.amp_list = list(self.amp)
        self.length = len(self.freq_list)

    def _Draw_FFT(self):
        ''' draw by matplotlib '''
        # print("[Called!] draw")
        plt.plot(self.freq_list, self.amp_list, color = "blue", linestyle = 'solid')
        plt.xscale("log")
        plt.title(self.obj_name + "'s power spectol", y = -0.25)
        plt.xlim(1,100)
        plt.xlabel('freq [Hz]')
        plt.ylabel('Amp[N]')
        plt.savefig("fft_fig/" + FILE + "_" + self.obj_name + "_" + str(self.t_from) + "_" + str(self.t_to) + ".png", bbox_inches='tight')
        # plt.show()
        plt.clf()
        plt.close()
        
    def _Save_CSV(self):
        ''' write  '''
        # print("[Called!] Save in csv")
        for i in range(self.length):
            self.output.write(f"{self.freq_list[i]:.4f},{self.amp_list[i]:.4f}\n")
        self.output.close()
    
    def FFT_MAIN(self) -> list:
        ''' When it called, last datalist is trached '''
        self._Read_data()
        self._FFT()
        self._Save_CSV()
        self._Draw_FFT()

plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.size'] = 20
plt.rcParams['xtick.direction'] = 'in'  # x軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')
plt.rcParams['ytick.direction'] = 'in'

ST = 0.0001 # 元データのscvデータサンプリング時間

PATH = "data/" + FILE + ".csv"
df = pd.read_csv(PATH, header = 0)

# for i in range(2):
#     for j in range(2):
#         NEW_FILE = FILE + "-fre_test_FFT_"+ obj_list[i] + str(t_list[j]) + "_" + str(t_list[j + 1])
#         NEW_PATH = "data/" + NEW_FILE + ".csv"
#         ft = FOURIER(ST, df, obj_list[i], NEW_PATH, t_list[j], t_list[j + 1])
#         ft.FFT_MAIN()

NEW_FILE = FILE + "_FFT_" + str(from_t) + "_" + str(to_t)
NEW_PATH = "data/" + NEW_FILE + ".csv"

ft = FOURIER(ST, df, " leader_f_dob", NEW_PATH, from_t, to_t)
ft.FFT_MAIN()
