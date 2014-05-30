#ifndef IPVLIDATOR_H
#define IPVLIDATOR_H

#include <QDialog>

class QLineEdit;

class IpValidator : public QDialog
{
    Q_OBJECT
public:
    IpValidator(QWidget * parent);
protected:
    bool validateIp(QLineEdit * ipLineEdit, bool allowNull, bool quite);
};

#endif // IPVLIDATOR_H
