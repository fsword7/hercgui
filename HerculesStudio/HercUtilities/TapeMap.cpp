#include "TapeMap.h"

#include "HerculesStudio.h"
#include "SystemUtils.h"
#include "Preferences.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"
#include "StringTokenizer.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

#include <csignal>

TapeMap::TapeMap(QWidget *parent)
    : QDialog(parent), mPid(-1)
{
	ui.setupUi(this);

    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(ui.browseFileButton, SIGNAL(clicked()), this, SLOT(browseFileClicked()));
    connect(this, SIGNAL(error()), this, SLOT(errorSlot()));
}

TapeMap::~TapeMap()
{

}

void TapeMap::runClicked()
{
    if (mPid > 0)
    {
        kill(mPid, SIGKILL);
        QMessageBox::warning(this, "tapemap",
                                            "tapemap operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        mPid=-1;
        ui.runButton->setText("Run");
        return;
    }
    if (ui.filename->text().isEmpty())
    {
        QMessageBox::warning(this, "tapemap", "Please specify tape file to process",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (!QFile::exists(ui.filename->text()))
    {
        QMessageBox::warning(this, "tapemap", "The specified file does not exist",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    std::vector<std::string> parameters;

    parameters.push_back(ui.filename->text().toStdString());

    parameters.push_back("EXTERNALGUI");
    std::string command = "tapemap";

    mExecutor = new UtilityExecutor();
    std::string path = "";
    mPid = mExecutor->run(command, path, parameters);
    int fileNo = mExecutor->getPipeOut();
    FILE * file = fdopen(fileNo,"r");
    UtilityRunner * runner = new UtilityRunner(file);
    runner->start();
    fileNo = mExecutor->getPipeError();
    file = fdopen(fileNo,"r");
    UtilityRunner * runnerError = new UtilityRunner(file);
    runnerError->start();

    connect(runner, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    connect(runnerError, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    ui.runButton->setText("Stop");

    fileNo = mExecutor->getPipeIn();
    FILE * fileIn = fdopen(fileNo,"w");
    putc('s', fileIn);
    fclose(fileIn);

    mCheckCount = 0;
    QTimer::singleShot(1000, this, SLOT(timeout()));
}

void TapeMap::browseFileClicked()
{
	QString dir = ui.filename->text();
	if (dir.isEmpty())
		dir = Preferences::getInstance().configDir().c_str();
    QString s = QFileDialog::getOpenFileName(this,"Browse for tape to map",dir);
    ui.filename->setText(s);
}

void TapeMap::exitClicked()
{
    deleteLater();
}

void TapeMap::runnerError(const QString& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line.toStdString(), pos, lastPos, " =\t\n");
    if ( (word == "HHCDU009E" && line.toStdString().find("EXTERNALGUI")!= std::string::npos) ||
    	 (word == "IPOS") )
        return;
    if (QString::compare(line.left(12),"End of tape.") == 0)
    {
    	deleteLater();
    }
    emit output(line);
}

void TapeMap::errorSlot()
{
	QMessageBox::warning(this, "tapemap",
								"tapemap failed. Please check the log for details",
								QMessageBox::Ok,
								QMessageBox::NoButton);
	deleteLater();
}

void TapeMap::timeout()
{
    if (mPid > 0)
    {
    	if (!mExecutor->evaluateProcess(mPid))
    	{
    		mCheckCount++;
    		if (mCheckCount >= 5)
    		{
				QMessageBox::warning(this, "tapemap",
							"tapemap operation is taking too long. Please check the log for errors",
							QMessageBox::Ok,
							QMessageBox::NoButton);
				deleteLater();
    		}
    	}
    	else
    	{
    		mPid=-1;
    	    ui.runButton->setText("Run");
    	    emit error();
    	}
    }
}
