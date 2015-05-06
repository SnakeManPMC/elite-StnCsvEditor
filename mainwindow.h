#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_Previous_clicked();

	void on_Next_clicked();

	void on_AddStation_clicked();

	void on_RedItOut_clicked();

	void on_actionLoad_CSV_triggered();

	void on_actionSave_CSV_triggered();

	void on_actionLoad_Red_List_triggered();

	void on_actionSave_Red_List_triggered();

	void on_actionSave_Ignore_List_triggered();

	void on_DistanceToStarLs_editingFinished();

	void on_PadMedium_clicked();

	void on_PadLarge_clicked();

	void on_AlwaysOnTop_clicked(bool checked);

	void on_Ignore_clicked();

	void on_actionLoad_Ignore_List_triggered();

	void on_DoneStations_clicked();

	void on_actionSave_Done_Stations_triggered();

	void on_MarketUnknown_clicked();

	void on_MarketYes_clicked();

	void on_MarketNo_clicked();

	void on_ClipboardSystemName_clicked();

	void on_ClipboardStationName_clicked();

private:
	Ui::MainWindow *ui;
	QList<QString> stations,redstations,ignores,donestations;
	int i;
	void ParseLine(QString line);
	void SystemNameAutoClipboard(QString line);
	QString StnCsvEditorVersion;
};

#endif // MAINWINDOW_H
