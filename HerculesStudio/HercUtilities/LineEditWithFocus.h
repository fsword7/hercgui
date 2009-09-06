/*
 *  File:       LineEditWithFocus.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 */

#ifndef LINEEDITWITHFOCUS_H_
#define LINEEDITWITHFOCUS_H_

#include <QLineEdit>

class LineEditWithFocus : public QLineEdit
{
    Q_OBJECT
public:
    LineEditWithFocus(QWidget* parent=0);
    virtual ~LineEditWithFocus();

protected:
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

signals:
    void FocusGained();
    void FocusLost();
};

#endif /* LINEEDITWITHFOCUS_H_ */
