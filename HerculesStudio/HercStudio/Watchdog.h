/*
 *  File:       Watchdog.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include <QThread>

class Watchdog : public QThread
{
	Q_OBJECT
public:
	Watchdog(int herculesPid, bool child);
	virtual ~Watchdog();

	virtual void run();

signals:
	void HerculesEnded();

private:
	int mHerculesPid;
	bool mChild;

    bool processIsRunning(int pid); // TODO: merge with NamedPipe (same function!)
    bool fileExists(const std::string & pid); // TODO: merge with NamedPipe (same function!)
};

#endif /* WATCHDOG_H_ */
