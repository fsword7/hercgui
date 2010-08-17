#ifndef TAPEMAP_H
#define TAPEMAP_H

#include <QtGui/QDialog>
#include "ui_TapeMap.h"

#include <cstdio>
class UtilityExecutor;

class TapeMap : public QDialog
{
    Q_OBJECT

public:
    TapeMap(QWidget *parent = 0);
    ~TapeMap();

private slots:
    void exitClicked();
    void runClicked();
    void browseFileClicked();
    void runnerError(const QString& errorLine);
    void errorSlot();
    void timeout();

private:
    Ui::TapeMapClass ui;
    int     mPid;
    int		mCheckCount;
    UtilityExecutor * mExecutor;

signals:
        void output(QString line);
        void error();
};

#endif // TAPEMAP_H
