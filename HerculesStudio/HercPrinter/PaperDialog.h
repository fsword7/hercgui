#ifndef PAPERDIALOG_H
#define PAPERDIALOG_H

#include <QDialog>

namespace Ui {
class PaperDialog;
}

class PaperDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaperDialog(QWidget *parent = 0);
    ~PaperDialog();

private:
    Ui::PaperDialog *ui;
};

#endif // PAPERDIALOG_H
