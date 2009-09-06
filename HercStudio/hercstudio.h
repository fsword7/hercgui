#ifndef HERCSTUDIO_H
#define HERCSTUDIO_H

#include <QtGui/QWidget>
#include "ui_hercstudio.h"

class HercStudio : public QWidget
{
    Q_OBJECT

public:
    HercStudio(QWidget *parent = 0);
    ~HercStudio();

private:
    Ui::HercStudioClass ui;
};

#endif // HERCSTUDIO_H
