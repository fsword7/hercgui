#ifndef PRINTERITEM_H
#define PRINTERITEM_H

#include <QString>

class PrinterItem
{
public:
    PrinterItem(); //TODO convert to itemized cnstructor

public:
    QString mName;
    QString mIp;
    qint16 mPort;
    QString mStationery;
    int mLinesPerInch;
    int mFontSize;
    QString mSpoolDir;
    QString mDecolationName;

};
#endif // PRINTERITEM_H
