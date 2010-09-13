/*
 *  File:       TapeSplt.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 31, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id$
 *
 *  tapesplt utility object
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
#ifndef TAPESPLT_H
#define TAPESPLT_H

#include "GenericUtility.h"
#include "ui_TapeSplt.h"
#include <QtGui/QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QAbstractTableModel>
#include <QLabel>
#include <QSpinBox>

#include <vector>

class TapeTableView : public QTableView
{
	Q_OBJECT
	TapeTableView(QWidget * parent = 0);
	virtual ~TapeTableView();

};

class TapeSpltSubDlg : public QDialog
{	
	Q_OBJECT
public:
	TapeSpltSubDlg(QWidget *parent = 0);
	virtual ~TapeSpltSubDlg();
	
private:
	QLabel *label;
	QLineEdit *lineEdit;
	QPushButton *selOutputButton;
	QSpinBox *spinBox;
	QLabel *label_2;
	QPushButton *addButton;
	QPushButton *cancelButton;
	TapeTableView *tableView;

	void setupUi();

private slots:
	void selectOutputSlot();
	void addSlot();
	void cancelSlot();

signals:
    void fileNameSet(QString, int);
};

#define HTableType std::vector<QStringList>
class HTableModel : public QAbstractTableModel
{
public:
	HTableModel(QObject * parent = 0);
	virtual ~HTableModel();

	virtual int      rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int      columnCount(const QModelIndex &parent = QModelIndex()) const;
	bool             insertRow(int row, const QModelIndex &, QString, QString);
	bool			 removeRow ( int row, const QModelIndex & parent = QModelIndex() );
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant rowData(const QModelIndex &index) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	bool             setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
private:
	HTableType mTable;

};

class TapeSplt : public GenericUtility
{
    Q_OBJECT

public:
    TapeSplt(QWidget *parent = 0);
    ~TapeSplt();

private:
	Ui::TapeSpltClass ui;
	HTableModel *mModel;
	int mMaxFile;

	virtual void finishedSlot();
	QString genLine(QString file, int blocks);

private slots:
	void runClicked();
	void cancelClicked();
	void selectInputClicked();
	void addClicked();
	void deleteClicked();
	void downClicked();
	void upClicked();
	void fileNameSetSlot(QString, int);
};

#endif // TAPESPLT_H
