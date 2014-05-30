#ifndef FIELDTIP_H
#define FIELDTIP_H

#include <QWidget>
#include <QTimer>

class FieldTip : public QWidget
{
    Q_OBJECT
public:
    explicit FieldTip(QWidget *parent = 0, bool top=false);
    virtual ~FieldTip();
    void showMessage(const QString& message, int timeout=10000);
    void setOwner(QWidget* widget) { attachedField = widget; }

protected:
   virtual void paintEvent(QPaintEvent*);

signals:

public slots:

private:
    bool mTop;
    QWidget *attachedField;
    QTimer expireTimer;

    void calculatePosition();
    QSize findBestSize(const QFontMetrics& fontMetrics, const QString& message);

};

#endif // FIELDTIP_H
