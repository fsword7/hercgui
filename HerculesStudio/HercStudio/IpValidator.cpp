#include "IpValidator.h"
#include "HerculesStudio.h"

#include <QLineEdit>
#include <QMessageBox>

IpValidator::IpValidator(QWidget * parent) :
QDialog(parent)
{
}

bool IpValidator::validateIp(QLineEdit * ipLineEdit, bool allowNull)
{
    const std::string ip = ipLineEdit->text().toStdString();
    bool ret = true;
    if (allowNull && ip.compare("...") == 0)
        return ret;
    QString qip = ip.c_str();
    int pos=0;
    for (int i=0; i<4 && ret; i++)
    {
        hOutDebug(5, "IP:" << ip << " pos:" << pos << " i=" << i << std::endl);
        if (i>0)
        {
            if (qip.toLatin1().data()[pos] != '.')
            {
                ret = false;
                break;
            }
            pos++;
        }
        int num = 0;
        int ppos=pos;
        while(ip[pos] >= '0' && ip[pos] <= '9')
        {
            num = num*10 + ip[pos] - '0';
            pos++;
        }
        if (pos == ppos || num > 255)
        {
            ret = false;
            break;
        }
    }
    if (!ret)
    {
        QString msg = ip.c_str();
        msg += " is an illegel IP address";
        QMessageBox::warning(this, "Illegal IP address", msg , QMessageBox::Ok);
        ipLineEdit->setFocus();
        return false;
    }
    return true;
}
