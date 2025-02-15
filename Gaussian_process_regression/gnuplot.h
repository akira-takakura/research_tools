#define GNUPLOT_PATH		"C:/PROGRA~1/gnuplot/bin/gnuplot.exe"
#define CURRENT_DIRECTORY	"C:/Users/staya/OneDrive - keio.jp/Research/Nozaki_Lab/simulations/research_tools/Gaussian_process_regression/"
#define OUTPUT_DIRECTORY	"fig/"

#define DATA_EXTENSION	".dat"
//#define DATA_EXTENSION	".csv"
#define FIG_EXTENSION	".png"
//#define FIG_EXTENSION	".eps"

#define FONT "'Times,15'"

/* Gnuplotによる描画 */
void Gnuplot(char* name) {
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
	fprintf(gp, "set xlabel '%s' font %s\n", "Stiffness [N/m]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Position [mm]", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, 8000.0);
	fprintf(gp, "set yrange [%lf:%lf]\n", -10.0, 60.0);
	fprintf(gp, "plot f using 2 : 7 w l lw 1 lc 'red' notitle, f using 3 : 8 w l lw 1 lc 'red' notitle, f using 4 : 9 w l lw 1 lc 'red' notitle, f using 5 : 10 w l lw 1 lc 'red' notitle, f using 6 : 11 w l lw 1 lc 'red' title 'Data', f using 17 : 18 w l lw 1 lc 'blue' title 'GPR'\ \n");

	// 環境剛性と力
	fprintf(gp, "set output '%s%s%s'\n", OUTPUT_DIRECTORY, "stiff_force", FIG_EXTENSION);
	fprintf(gp, "set xlabel '%s' font %s\n", "Stiffness [N/m]", FONT);
	fprintf(gp, "set ylabel '%s' font %s\n", "Force [mN]", FONT);
	fprintf(gp, "set xrange [%lf:%lf]\n", 0.0, 8000.0);
	fprintf(gp, "set yrange [%lf:%lf]\n", 0.0, 24000.0);
	fprintf(gp, "plot f using 2 : 12 w l lw 1 lc 'red' notitle, f using 3 : 13 w l lw 1 lc 'red' notitle, f using 4 : 14 w l lw 1 lc 'red' notitle, f using 5 : 15 w l lw 1 lc 'red' notitle, f using 6 : 16 w l lw 1 lc 'red' title 'Data', f using 17 : 19 w l lw 1 lc 'blue' title 'GPR'\ \n");

	/* 終了コマンド */
	fflush(gp);
	system("pause");
	fprintf(gp, "exit\n");
	_pclose(gp);
}