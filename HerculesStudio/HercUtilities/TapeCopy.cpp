#include "TapeCopy.h"
#include "Preferences.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"
#include "StringTokenizer.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTimer>

#include <csignal>


TapeCopy::TapeCopy(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	mUpperGeometry = ui.FileBox->geometry();
	mLowerGeometry = ui.ScsiBox->geometry();
	connect(ui.scsiToFileRadio, SIGNAL(clicked()), this, SLOT(scsiToTape()));
	connect(ui.fileToScsiRadio, SIGNAL(clicked()), this, SLOT(tapeToScsi()));
	connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(ui.browseFileButton, SIGNAL(clicked()), this, SLOT(browseFileClicked()));
    connect(this, SIGNAL(error()), this, SLOT(errorSlot()));
}

TapeCopy::~TapeCopy()
{

}

void TapeCopy::runClicked()
{
    if (mPid > 0)
    {
        kill(mPid, SIGKILL);
        QMessageBox::warning(this, "tapecopy",
                                            "tapecopy operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        mPid=-1;
        ui.runButton->setText("Run");
        return;
    }
    if (ui.filename->text().isEmpty())
    {
        QMessageBox::warning(this, "tapecopy", "Please specify tape file to process",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (!QFile::exists(ui.filename->text()))
    {
        QMessageBox::warning(this, "tapecopy", "The specified file does not exist",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    std::vector<std::string> parameters;

    parameters.push_back(ui.filename->text().toStdString());

    std::string scsi="/dev/st";
    scsi+=ui.spinBox->text().toStdString();
    parameters.push_back(scsi.c_str());

    parameters.push_back("EXTERNALGUI");
    std::string command = "tapecopy";

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

void TapeCopy::browseFileClicked()
{
	QString dir = ui.filename->text();
	if (dir.isEmpty())
		dir = Preferences::getInstance().configDir().c_str();
    QString s = QFileDialog::getOpenFileName(this,"Browse for tape to map",dir);
    ui.filename->setText(s);
}

void TapeCopy::cancelClicked()
{
	deleteLater();
}

void TapeCopy::tapeToScsi()
{
	ui.FileBox->setGeometry(mUpperGeometry);
	ui.ScsiBox->setGeometry(mLowerGeometry);
	ui.FileBox->setTitle("Input Tape");
	ui.ScsiBox->setTitle("Output Tape");
}

void TapeCopy::scsiToTape()
{
	ui.FileBox->setGeometry(mLowerGeometry);
	ui.ScsiBox->setGeometry(mUpperGeometry);
	ui.FileBox->setTitle("Output Tape");
	ui.ScsiBox->setTitle("Input Tape");
}
void TapeCopy::runnerError(const QString& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line.toStdString(), pos, lastPos, " =\t\n");
    if ( (word == "HHCDU009E" && line.toStdString().find("EXTERNALGUI")!= std::string::npos) ||
    	 (word == "IPOS") )
        return;
    if (word == "HHCTC000I")
    {
		mPid=-1;
	    ui.runButton->setText("Run");
    	emit error();
    }
    emit output(line);
}

void TapeCopy::errorSlot()
{
	QMessageBox::warning(this, "tapemap",
								"tapemap failed. Please check the log for details",
								QMessageBox::Ok,
								QMessageBox::NoButton);
}

void TapeCopy::timeout()
{
    if (mPid > 0)
    {
    	if (!mExecutor->evaluateProcess(mPid))
    	{
    		mCheckCount++;
    		if (mCheckCount >= 5)
    		{
				QMessageBox::warning(this, "tapecopy",
							"tapecopy operation is taking too long. Please check the log for details",
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

