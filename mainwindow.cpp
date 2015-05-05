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
	setWindowTitle("StnCsvEditor PMC " + StnCsvEditorVersion);

	i = 0;

	// debug load the shit straight up :)
	// red list source CSV
	QFile file("d:\\coding\\test_files\\stations_maddavo.csv");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open source CSV file"), file.errorString());
	}
	QTextStream in(&file);
	while (!in.atEnd())
	{
		lines.push_back(in.readLine());
	}
	file.close();
	ui->StationText->setText(lines[i]);
	ParseLine(lines[i]);
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
		ui->StationText->setText(lines[i]);
		ParseLine(lines[i]);
	}
	else
		ui->textEdit->append("At digit 1, no lower lines available. Please use Next to browse forward.");
}


void MainWindow::on_Next_clicked()
{
	// next system button
	i++;
	ui->StationText->setText(lines[i]);
	ParseLine(lines[i]);
}

void MainWindow::on_AddStation_clicked()
{
	// add station button
}


void MainWindow::on_RedItOut_clicked()
{
	// put station in the red list
	redstations.push_back(lines[i]);
	// remove station from lines list
	lines.removeAt(i);

	// refresh our station text with the new i station number
	ui->StationText->setText(lines[i]);
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
			lines.append(in.readLine());
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
	for(int cx = 0; cx < lines.count(); cx++) out << lines[cx] + "\n";
	file.close();
	ui->textEdit->append("Wrote " + QString::number(lines.count()) + " into " + fileName);
}


void MainWindow::on_actionLoad_Red_List_triggered()
{

}


void MainWindow::ParseLine(QString line)
{
	QStringList list;
	list = line.split(",");
	bool ok;
	ui->DistanceToStarLs->setValue(list[2].toInt(&ok, 10));
	ui->textEdit->append("Distance from star: " + list[2]);
	ui->textEdit->append("Landing Pad: " + list[4]);
	ui->textEdit->append("Market: " + list[5]);

	if (list[4] == "'L'")
	{
		ui->textEdit->append("list[4] == L");
		ui->PadLarge->setChecked(true);
		ui->PadMedium->setChecked(false);
	}

	if (list[4] == "'M'")
	{
		ui->textEdit->append("list[4] == M");
		ui->PadLarge->setChecked(false);
		ui->PadMedium->setChecked(true);
	}

	if (list[4] == "'?'")
	{
		ui->textEdit->append("list[4] == ?");
		ui->PadLarge->setChecked(false);
		ui->PadMedium->setChecked(false);
	}

	if (list[5] == "'?'")
	{
		ui->textEdit->append("list[5] == ?");
		ui->Market->setCurrentIndex(0);
	}

	if (list[5] == "'Y'")
	{
		ui->textEdit->append("list[5] == Y");
		ui->Market->setCurrentIndex(1);
	}

	if (list[5] == "'N'")
	{
		ui->textEdit->append("list[5] == N");
		ui->Market->setCurrentIndex(2);
	}

	// if we automatically clipboard system name
	if (ui->SysNamAutoClipboard->isChecked()) SystemNameAutoClipboard(list[0]);
}


// market dropdown was activated by user?
// unfortunately this is launched also when we internally change the data, hehe
void MainWindow::on_Market_currentIndexChanged(int index)
{
	if (index == 0) ui->textEdit->append("Well index is 0 hehe");
	ui->textEdit->append("You changed Market info.");
}


// distance from star, editing has finished
void MainWindow::on_DistanceToStarLs_editingFinished()
{
	ui->textEdit->append("distance from star editing has finished!");
}


// landing pad set to Medium
void MainWindow::on_PadMedium_clicked()
{
	ui->textEdit->append("landing pad set to medium!");
}


// landing pad set to large
void MainWindow::on_PadLarge_clicked()
{
	ui->textEdit->append("landing pad set to large!");
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
