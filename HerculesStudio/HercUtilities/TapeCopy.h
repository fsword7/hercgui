#ifndef HETMAP_H
#define HETMAP_H

#include <QtGui/QDialog>
#include "ui_TapeCopy.h"

class UtilityExecutor;
class TapeCopy : public QDialog
{
    Q_OBJECT

public:
    TapeCopy(QWidget *parent = 0);
    ~TapeCopy();

private:
    Ui::TapeCopyClass ui;
    QRect mUpperGeometry, mLowerGeometry;
    int mPid;
    int		mCheckCount;
    UtilityExecutor * mExecutor;

public slots:
	void runClicked();
	void cancelClicked();
	void browseFileClicked();
    void runnerError(const QString& errorLine);
    void timeout();
	void tapeToScsi();
	void scsiToTape();
	void errorSlot();

signals:
    void output(QString line);
    void error();
};

#endif // HETMAP_H
