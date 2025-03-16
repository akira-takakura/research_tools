#define GNUPLOT_PATH		"C:/PROGRA~1/gnuplot/bin/gnuplot.exe"
#define CURRENT_DIRECTORY	"C:/Users/staya/OneDrive - keio.jp/Research/Nozaki_Lab/simulations/research_tools/Gaussian_process_regression/"
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

	// 環境剛性と位置
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "stiff_position", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Stiffness [kN/m]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Position [mm]", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, 8.0);
	fprintf(gp, "set yrange [%lf:%lf]\n", -10.0, 60.0);
	fprintf(gp, "plot f using 2 : 7 w l lw 1 lc 'red' notitle, f using 3 : 8 w l lw 1 lc 'red' notitle, f using 4 : 9 w l lw 1 lc 'red' notitle, f using 5 : 10 w l lw 1 lc 'red' notitle, f using 6 : 11 w l lw 1 lc 'red' title 'Data', f using 17 : 18 w l lw 1 lc 'blue' title 'GPR'\ \n");

	// 環境剛性と力
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "stiff_force", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Stiffness [kN/m]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Force [N]", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, 8.0);
	fprintf(gp, "set yrange [%lf:%lf]\n", 0.0, 24.0);
	fprintf(gp, "plot f using 2 : 12 w l lw 1 lc 'red' notitle, f using 3 : 13 w l lw 1 lc 'red' notitle, f using 4 : 14 w l lw 1 lc 'red' notitle, f using 5 : 15 w l lw 1 lc 'red' notitle, f using 6 : 16 w l lw 1 lc 'red' title 'Data', f using 17 : 19 w l lw 1 lc 'blue' title 'GPR'\ \n");
	
	// ハイパーパラメータ推移 (位置)
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "hyperparam_posi", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Theta (Position)", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -10.0, 600.0);
	fprintf(gp, "plot f using 1 : 22 w l lw 1 lc 'red' title 'theta(1)', f using 1 : 23 w l lw 1 lc 'green' title 'theta(2)', f using 1 : 24 w l lw 1 lc 'blue' title 'theta(3)'\ \n");

	// ハイパーパラメータ推移 (力)
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "hyperparam_force", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Theta (Force)", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -10.0, 500.0);
	fprintf(gp, "plot f using 1 : 25 w l lw 1 lc 'red' title 'theta(1)', f using 1 : 26 w l lw 1 lc 'green' title 'theta(2)', f using 1 : 27 w l lw 1 lc 'blue' title 'theta(3)'\ \n");
	
	// ハイパーパラメータ微分 (位置)
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "hyperparam_diff_posi", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "dTheta (Position)", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -10.0, 10.0);
	fprintf(gp, "plot f using 1 : 32 w l lw 1 lc 'red' title 'theta(1)', f using 1 : 33 w l lw 1 lc 'green' title 'theta(2)', f using 1 : 34 w l lw 1 lc 'blue' title 'theta(3)'\ \n");

	// ハイパーパラメータ微分 (力)
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "hyperparam_diff_force", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "dTheta (Force)", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -10.0, 10.0);
	fprintf(gp, "plot f using 1 : 35 w l lw 1 lc 'red' title 'theta(1)', f using 1 : 36 w l lw 1 lc 'green' title 'theta(2)', f using 1 : 37 w l lw 1 lc 'blue' title 'theta(3)'\ \n");

	// 分散
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "Variance", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Variance", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -10.0, 600.0);
	fprintf(gp, "plot f using 1 : 28 w l lw 1 lc 'blue' title 'Position', f using 1 : 29 w l lw 1 lc 'green' title 'Force'\ \n");

	// 尤度関数
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "likelihood", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Time [s]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Liklihood function", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, t_end);
	fprintf(gp, "set yrange [%lf:%lf]\n", -50.0, 0.0);
	fprintf(gp, "plot f using 1 : 30 w l lw 1 lc 'blue' title 'Position', f using 1 : 31 w l lw 1 lc 'green' title 'Force'\ \n");

	/* 終了コマンド */
	fflush(gp);
	system("pause");
	fprintf(gp, "exit\n");
	_pclose(gp);
}