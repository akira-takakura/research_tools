#define GNUPLOT_PATH		"C:/PROGRA~1/gnuplot/bin/gnuplot.exe"
#define CURRENT_DIRECTORY	"C:/Users/staya/OneDrive - keio.jp/Research/Nozaki_Lab/simulations/research_tools/Gaussian_process_regression_test/"
#define OUTPUT_DIRECTORY	"fig/"

#define DATA_EXTENSION	".dat"
//#define DATA_EXTENSION	".csv"
#define FIG_EXTENSION	".png"
//#define FIG_EXTENSION	".eps"

#define FONT "'Times,15'"

/* Gnuplotによる描画 */
void Gnuplot(char* name, double t_end) {
	printf("Gnuplot[Called!]");
	FILE* gp;
	if ((gp = _popen(GNUPLOT_PATH, "w")) == NULL) {
		fprintf(stderr, "ファイルが見つかりません %s.", GNUPLOT_PATH);
	}
	/* 本文 */
	/* 事前準備 */
	fprintf(gp, "reset \n");
	fprintf(gp, "cd  '%s'\n", CURRENT_DIRECTORY);
	fprintf(gp, "set terminal pngcairo\n");		// png出力のための設定

	fprintf(gp, "set key top right font %s \n", FONT);
	fprintf(gp, "set xlabel 'Time [s]' font %s \n", FONT);
	fprintf(gp, "set style rect fc lt -1 fs solid 0.15 noborder \n");
	// 軸設定
	fprintf(gp, "set tics font %s \n", FONT);
	fprintf(gp, "set xtics scale %lf \n", 2.0);
	fprintf(gp, "set mxtics \n");
	fprintf(gp, "set xtics scale %lf \n", 2.0);
	fprintf(gp, "set mytics\n");
	// 描画範囲
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, 10.0);
	// データ区切りの指定
	//fprintf(gp, "set datafile separator "," \n");	// csvなら必須

/* 描画 */
	fprintf(gp, "f = '%s'\n", name);	// 描画データ指定

	// ハイパーパラメータの真値との誤差
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "hyperparam_tilde", FIG_EXTENSION);
	//fprintf(gp, "set xlabel '%s' font %s\n", "Step number [s]", FONT);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Theta", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -6.0, 2.0);
	fprintf(gp, "plot f using 1 : 3 w l lw 1 lc 'red' title 'theta(1) tilde', f using 1 : 4 w l lw 1 lc 'green' title 'theta(2) tilde', f using 1 : 5 w l lw 1 lc 'blue' title 'theta(3) tilde'\ \n");

	// ハイパーパラメータの値
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "hyperparam", FIG_EXTENSION);
	//fprintf(gp, "set xlabel '%s' font %s\n", "Step number [s]", FONT);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Theta", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -1.0, 10.0);
	fprintf(gp, "plot f using 1 : 9 w l lw 1 lc 'red' title 'theta(1)', f using 1 : 10 w l lw 1 lc 'green' title 'theta(2)', f using 1 : 11 w l lw 1 lc 'blue' title 'theta(3)'\ \n");

	// ハイパーパラメータ勾配
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "hyperparam_diff", FIG_EXTENSION);
	//fprintf(gp, "set xlabel '%s' font %s\n", "Step number [s]", FONT);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Theta", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -3.0, 10.0);
	fprintf(gp, "plot f using 1 : 6 w l lw 1 lc 'red' title 'dtheta(1)', f using 1 : 7 w l lw 1 lc 'green' title 'dtheta(2)', f using 1 : 8 w l lw 1 lc 'blue' title 'dtheta(3)'\ \n");

	// 尤度関数
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "likelihood", FIG_EXTENSION);
	//fprintf(gp, "set xlabel '%s' font %s\n", "Step number[]", FONT);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Liklihood function", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -8.0, 0.0);
	fprintf(gp, "plot f using 1 : 2 w l lw 1 lc 'blue' title 'Log likelihood function'\ \n");

	/* 終了コマンド */
	fflush(gp);
	system("pause");
	fprintf(gp, "exit\n");
	_pclose(gp);
}