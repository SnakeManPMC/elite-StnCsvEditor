#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// our softwares version
	StnCsvEditorVersion = "v0.0.0";
	setWindowTitle("StnCsvEditor " + StnCsvEditorVersion + " by PMC");

	i = 1;

	/*
	// debug load the shit straight up :)
	// red list source CSV
	QFile file("d://test_files//stations_maddavo.csv");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open source CSV file"), file.errorString());
	}
	QTextStream in(&file);
	while (!in.atEnd())
	{
		stations.push_back(in.readLine());
	}
	file.close();
	ui->StationText->setText(stations[i]);
	ParseLine(stations[i]);
	*/
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_Previous_clicked()
{
	// previous system button

	// first check that we dont go to negative numbers
	if (i > 1)
	{
		i--;
		ui->StationText->setText(stations[i]);
		ParseLine(stations[i]);
	}
	else
		ui->textEdit->append("At digit 1, no lower lines available. Please use Next to browse forward.");
}


void MainWindow::on_Next_clicked()
{
	// next system button
	if (i < stations.count())
	{
		i++;
		ui->StationText->setText(stations[i]);
		ParseLine(stations[i]);
	}
	else
		ui->textEdit->append("At last digit, no more lines available. Please use Previous to browse backward.");
}


// add station button
void MainWindow::on_AddStation_clicked()
{
	// 1) has to add automatically full "line" template
	// 2) same system name that we currently are in i
	// 3) assume large station and has market
	// 4) dialog for station name?
	if (!ui->SystemText->text().size() == 0)
	{
		QString line = stations[i];
		QStringList list = line.split(",");
		stations.insert(i, "New Station Template");
		stations[i] = list[0] + ",'" + ui->SystemText->text() + "',0,'?','?','?','?','2015-05-06 06:06:06','?','?','?','?'";

		ui->SystemText->clear();
		// display our new station
		i--;
		on_Next_clicked();

		ui->textEdit->append("New station created:\n" + stations[i]);
	}
	else
		ui->textEdit->append("You must type Station name into the first text box :)");
}


void MainWindow::on_RedItOut_clicked()
{
	// put station in the red list
	redstations.push_back(stations[i]);
	// remove station from lines list
	stations.removeAt(i);

	// refresh our station text with the new i station number
	ui->StationText->setText(stations[i]);
	ui->textEdit->append("Put station: " + redstations.last() + " in the red list.");
}


// load csv from the File menu
void MainWindow::on_actionLoad_CSV_triggered()
{
	QString fileName;
	QString line;

	fileName = QFileDialog::getOpenFileName(
		this,
		"Choose a CSV file to open",
		QString::null,
		QString::null);

	if (fileName.isEmpty())
	return;
	else
	{
		// red list source CSV
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::information(this, tr("Unable to open source CSV file"), file.errorString());
		}
		QTextStream in(&file);
		while (!in.atEnd())
		{
			stations.append(in.readLine());
		}
		file.close();
	}
}


// save csv from the File menu
void MainWindow::on_actionSave_CSV_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		//return false;
		exit(1);

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, tr("StnCsvEditor"),
				tr("Cannot write file %1:\n%2.")
				.arg(fileName)
				.arg(file.errorString()));
		//return false;
		exit(1);
	}

	QTextStream out(&file);
	for(int cx = 0; cx < stations.count(); cx++) out << stations[cx] + "\n";
	file.close();
	ui->textEdit->append("Wrote " + QString::number(stations.count()) + " into " + fileName);
}


void MainWindow::on_actionLoad_Red_List_triggered()
{
	QString fileName;
	QString line;

	fileName = QFileDialog::getOpenFileName(
		this,
		"Choose a CSV file to open",
		QString::null,
		QString::null);

	if (fileName.isEmpty())
	return;
	else
	{
		// red list source CSV
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::information(this, tr("Unable to open source CSV file"), file.errorString());
		}
		QTextStream in(&file);
		while (!in.atEnd())
		{
			redstations.append(in.readLine());
		}
		file.close();
	}
}


void MainWindow::ParseLine(QString line)
{
	QStringList list;
	list = line.split(",");
	bool ok;
	ui->DistanceToStarLs->setValue(list[2].toInt(&ok, 10));
	//ui->textEdit->append("Distance from star: " + list[2]);
	//ui->textEdit->append("Landing Pad: " + list[4]);
	//ui->textEdit->append("Market: " + list[5]);

	if (list[4] == "'L'")
	{
		//ui->textEdit->append("list[4] == L");
		ui->PadLarge->setChecked(true);
		ui->PadMedium->setChecked(false);
		ui->PadUnknown->setChecked(false);
	}

	if (list[4] == "'M'")
	{
		//ui->textEdit->append("list[4] == M");
		ui->PadLarge->setChecked(false);
		ui->PadMedium->setChecked(true);
		ui->PadUnknown->setChecked(false);
	}

	if (list[4] == "'?'")
	{
		//ui->textEdit->append("list[4] == ?");
		ui->PadLarge->setChecked(false);
		ui->PadMedium->setChecked(false);
		ui->PadUnknown->setChecked(true);
	}

	if (list[5] == "'?'")
	{
		//ui->textEdit->append("list[5] == ?");
		ui->MarketUnknown->setChecked(true);
		ui->MarketYes->setChecked(false);
		ui->MarketNo->setChecked(false);
	}

	if (list[5] == "'Y'")
	{
		//ui->textEdit->append("list[5] == Y");
		ui->MarketUnknown->setChecked(false);
		ui->MarketYes->setChecked(true);
		ui->MarketNo->setChecked(false);
	}

	if (list[5] == "'N'")
	{
		//ui->textEdit->append("list[5] == N");
		ui->MarketUnknown->setChecked(false);
		ui->MarketYes->setChecked(false);
		ui->MarketNo->setChecked(true);
	}

	// if we automatically clipboard system name
	if (ui->SysNamAutoClipboard->isChecked()) SystemNameAutoClipboard(list[0]);
}


// distance from star, editing has finished
void MainWindow::on_DistanceToStarLs_editingFinished()
{
	QStringList list;
	list = stations[i].split(",");
	stations[i] = list[0] + "," +
			list[1] + "," +
			QString::number(ui->DistanceToStarLs->value()) + "," +
			list[3] + "," +
			list[4] + "," +
			list[5] + "," +
			list[6] + "," +
			list[7] + "," +
			list[8] + "," +
			list[9] + "," +
			list[10] + "," +
			list[11];

	//ui->textEdit->append("distance from star editing has finished!");
}


// landing pad set to Medium
void MainWindow::on_PadMedium_clicked()
{
	QStringList list;
	list = stations[i].split(",");
	stations[i] = list[0] + "," +
			list[1] + "," +
			list[2] + "," +
			list[3] + "," +
			"'M'," +
			list[5] + "," +
			list[6] + "," +
			list[7] + "," +
			list[8] + "," +
			list[9] + "," +
			list[10] + "," +
			list[11];

	//ui->textEdit->append("landing pad set to medium!");
}


// landing pad set to large
void MainWindow::on_PadLarge_clicked()
{
	QStringList list;
	list = stations[i].split(",");
	stations[i] = list[0] + "," +
			list[1] + "," +
			list[2] + "," +
			list[3] + "," +
			"'L'," +
			"'Y'" + "," +
			list[6] + "," +
			list[7] + "," +
			list[8] + "," +
			list[9] + "," +
			list[10] + "," +
			list[11];

	// because every large pad station (starport) has market, we set that also to Y
	ui->MarketUnknown->setChecked(false);
	ui->MarketYes->setChecked(true);
	ui->MarketNo->setChecked(false);

	//ui->textEdit->append("landing pad set to large!");
}


// copy star system name to clipboard (so it can be pasted to IRC etc hehe)
void MainWindow::SystemNameAutoClipboard(QString line)
{
	// strip the beginning ' and ending ' from the string
	// end
	line.chop(1);
	// beginning
	line.remove(0,1);

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(line);
}


// always on top check box
void MainWindow::on_AlwaysOnTop_clicked(bool checked)
{
	Qt::WindowFlags flags = this->windowFlags();
	if (checked)
	{
	    flags ^= Qt::WindowStaysOnBottomHint;
	    flags |= Qt::WindowStaysOnTopHint;
	}
	else
	{
	    flags ^= Qt::WindowStaysOnTopHint;
	    flags |= Qt::WindowStaysOnBottomHint;
	}
	this->setWindowFlags(flags);
	this->show();
}


// ignore current station
void MainWindow::on_Ignore_clicked()
{
	ignores.push_back(stations[i]);
	stations.removeAt(i);

	on_Previous_clicked();
}


// save ignore list
void MainWindow::on_actionSave_Ignore_List_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		//return false;
		exit(1);

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, tr("StnCsvEditor"),
				tr("Cannot write file %1:\n%2.")
				.arg(fileName)
				.arg(file.errorString()));
		//return false;
		exit(1);
	}

	QTextStream out(&file);
	for(int cx = 0; cx < ignores.count(); cx++) out << ignores[cx] + "\n";
	file.close();
	ui->textEdit->append("Wrote " + QString::number(ignores.count()) + " into " + fileName);
}


// save red list
void MainWindow::on_actionSave_Red_List_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		//return false;
		exit(1);

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, tr("StnCsvEditor"),
				tr("Cannot write file %1:\n%2.")
				.arg(fileName)
				.arg(file.errorString()));
		//return false;
		exit(1);
	}

	QTextStream out(&file);
	for(int cx = 0; cx < redstations.count(); cx++) out << redstations[cx] + "\n";
	file.close();
	ui->textEdit->append("Wrote " + QString::number(redstations.count()) + " into " + fileName);
}


// load ignore list
void MainWindow::on_actionLoad_Ignore_List_triggered()
{
	QString fileName;
	QString line;

	fileName = QFileDialog::getOpenFileName(
		this,
		"Choose a CSV file to open",
		QString::null,
		QString::null);

	if (fileName.isEmpty())
	return;
	else
	{
		// ignore list source CSV
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::information(this, tr("Unable to open source CSV file"), file.errorString());
		}
		QTextStream in(&file);
		while (!in.atEnd())
		{
			ignores.append(in.readLine());
		}
		file.close();
	}
}


// big done button, add our station to DONE list and remove from stations list
void MainWindow::on_DoneStations_clicked()
{
	// if this is the first done station, lets add header also
	if (donestations.count() == 0) donestations.push_back(stations[0]);

	// and then our done station
	donestations.push_back(stations[i]);
	stations.removeAt(i);

	// we want to display the NEXT station, so decrease counter then hit next
	i--;
	on_Next_clicked();

	// show how many stations user has 'done' now
	ui->textEdit->append("Stations done: " + QString::number(donestations.count() - 1));
}


// save done stations, ready to be uploaded to Maddavo ;)
void MainWindow::on_actionSave_Done_Stations_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		//return false;
		exit(1);

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, tr("StnCsvEditor"),
				tr("Cannot write file %1:\n%2.")
				.arg(fileName)
				.arg(file.errorString()));
		//return false;
		exit(1);
	}

	QTextStream out(&file);
	for(int cx = 0; cx < donestations.count(); cx++) out << donestations[cx] + "\n";
	file.close();
	ui->textEdit->append("Wrote " + QString::number(donestations.count()) + " into " + fileName);
}


// market set to unknown
void MainWindow::on_MarketUnknown_clicked()
{
	QString market = "'?'";

	QStringList list;
	list = stations[i].split(",");
	stations[i] = list[0] + "," +
			list[1] + "," +
			list[2] + "," +
			list[3] + "," +
			list[4] + "," +
			market + "," +
			list[6] + "," +
			list[7] + "," +
			list[8] + "," +
			list[9] + "," +
			list[10] + "," +
			list[11];

	//ui->textEdit->append("on_MarketUnknown_clicked()");
}


// market set to yes
void MainWindow::on_MarketYes_clicked()
{
	QString market = "'Y'";

	QStringList list;
	list = stations[i].split(",");
	stations[i] = list[0] + "," +
			list[1] + "," +
			list[2] + "," +
			list[3] + "," +
			list[4] + "," +
			market + "," +
			list[6] + "," +
			list[7] + "," +
			list[8] + "," +
			list[9] + "," +
			list[10] + "," +
			list[11];

	//ui->textEdit->append("on_MarketYes_clicked()");
}


// market set to no
void MainWindow::on_MarketNo_clicked()
{
	QString market = "'N'";

	QStringList list;
	list = stations[i].split(",");
	stations[i] = list[0] + "," +
			list[1] + "," +
			list[2] + "," +
			list[3] + "," +
			list[4] + "," +
			market + "," +
			list[6] + "," +
			list[7] + "," +
			list[8] + "," +
			list[9] + "," +
			list[10] + "," +
			list[11];

	//ui->textEdit->append("on_MarketNo_clicked()");
}


// copy system name to clipboard
void MainWindow::on_ClipboardSystemName_clicked()
{
	QStringList list = stations[i].split(",");
	SystemNameAutoClipboard(list[0]);
}


// copy station name to clipboard
void MainWindow::on_ClipboardStationName_clicked()
{
	QStringList list = stations[i].split(",");
	SystemNameAutoClipboard(list[1]);
}
