/*
 *  File: PagePrinter.h
 *
 *  Author:     Jacob Dekel
 *  Created on: May 10, 2014
 *
 *  Copyright (c) 2009-2014 Jacob Dekel
 *  $Id$
 *
 *  This file contains the page formatting and PDF extension classes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PAGEPRINTER_H
#define PAGEPRINTER_H

#include "PrinterInterface.h"
#include "HerculesStudio.h"

#include <QPdfWriter>

class PrintRunner;

class PagePrinter : public GenericPagePrinter
{
    Q_OBJECT
public:
    PagePrinter(StationeryPtr &stationery, QPainter& painter);
    virtual void setBarsColor();
    virtual bool print(QByteArray);

protected:
    void setValues(bool);
    virtual void close();

private:
    const DecolationRulesPtr mRules;
    QString mStationeryName;
    QPainter* mPainter;

signals:
    void endPage();
};

class PdfPrinter : public GenericPagePrinter
{
public:
    PdfPrinter(const QString& dir, StationeryPtr& stationery, DecolationRulesPtr& decolation, int linesPerInch, int fontSize);
    virtual void setBarsColor();
    virtual bool print(QByteArray line);
    virtual void close();
    virtual bool endOfJob(QByteArray& line);

private:
    QString decodeFilename();
    bool doPrint(QByteArray& line);
    void setValues(bool eject);


    QMultiMap<int, AccountingField> mAccountingFields;
    QMap<QString, QString> mVariables;
    QPdfWriter * mPdf;
    QString mDir;
    QList<QByteArray> mFirstPage;
    int mFontSize;
    int mCurPage;
    int mCurLine;
    int mLastPages;
    int mLastPagesCountdown;
};

#endif // PAGEPRINTER_H

