#include "PaperDialog.h"
#include "ui_PaperDialog.h"
#include "HerculesStudio.h"

#include <QSpinBox>

#include <sstream>

class InchesSpinBox : public QSpinBox
{
public:
    InchesSpinBox(QWidget * parent=0);
protected:
    virtual int valueFromText(const QString &text) const;
    virtual QString textFromValue(int val) const;
private:
    mutable int mNumerator; // deniminator is always 16
};

InchesSpinBox::InchesSpinBox(QWidget *parent) : QSpinBox(parent)
{
    mNumerator = 8;
    setRange(1,32);
    setSingleStep(1);
    setValue(mNumerator);
}

int InchesSpinBox::valueFromText(const QString &text) const
{
    int slashPos = text.indexOf('/');
    bool ok;

    int numerator = text.left(slashPos).toInt(&ok);
    hOutDebug(0, "den=" << numerator << " OK=" << (ok? "Y" : "N"));
    if (!ok) return 8;

    int denominator = text.mid(slashPos+1).toInt(&ok);
    hOutDebug(0, "den=" << denominator << " OK=" << (ok? "Y" : "N"));
    if (!ok) return 8;

    if (denominator%2) return mNumerator;
    if (denominator == 2) mNumerator = numerator * 8;
    else if (denominator == 4) mNumerator = numerator * 4;
    else if (denominator == 8) mNumerator = numerator * 2;
    else mNumerator = numerator;
    return mNumerator;
}

QString InchesSpinBox::textFromValue(int val) const
{
    std::stringstream ss;
    hOutDebug(0, "Value=" << val);

    if ((val%16) == 0)
    {
        ss << (val/16) ;
    }
    else if ((val%8) == 0)
    {
        ss << (val/8) << "/" << "2";
    }
    else if ((val%4) == 0)
    {
        ss << (val/4) << "/" << "4";
    }
    else if ((val%2) == 0)
    {
        ss << (val/2) << "/" << "8";
    }
    else\
    {
        ss << val << "/" << "16";
    }
    //ss << "  (" << val << "/16)";

    hOutDebug(0, "text=" << ss.str());
    return ss.str().c_str();
}


PaperDialog::PaperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaperDialog)
{
    ui->setupUi(this);

    InchesSpinBox* topMarginSpinBox;
    topMarginSpinBox = new InchesSpinBox(this);
    topMarginSpinBox->setObjectName(QStringLiteral("topMarginSpinBox"));
    topMarginSpinBox->setGeometry(QRect(160, 250, 120, 31));
    topMarginSpinBox->setSingleStep(1);
}

PaperDialog::~PaperDialog()
{
    delete ui;
}

