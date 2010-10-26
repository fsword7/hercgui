/*
 *  File:       HerculesExecutor.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
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

#include "HerculesExecutor.h"
#include "HerculesStudio.h"
#include "Arguments.h"
#include "NamedPipe.h"

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <iostream>

int pipeLog[2];
int pipeStatus[2];
int pipeCommand[2];

HerculesExecutor::HerculesExecutor(QMainWindow & mainWindow, int pid)
: mMainWindow(mainWindow), mProcess(NULL), mPid(pid)
{

}

HerculesExecutor::~HerculesExecutor()
{

}

#ifndef hFramework
int HerculesExecutor::run(std::string configName, std::string herculesPath)
{
    int pid;
    int rc =0;

    pid = fork();
    if (pid < 0)
    	return -1;
    if (pid == 0)
    {
        std::string hercules = herculesPath;
        if (herculesPath.length() != 0 )
            hercules += "/";
        hercules += "hercules";
        outDebug(2, std::cout << "hercules:" << hercules << std::endl);

        FILE * fileOut = NamedPipe::getInstance().getHerculesStdout();
        rc = dup2(fileno(fileOut),fileno(stdout));

        FILE * fileErr = NamedPipe::getInstance().getHerculesStderr();
        rc = dup2(fileno(fileErr),fileno(stderr));

        FILE * fileIn = NamedPipe::getInstance().getHerculesStdin();
        if (fileIn == NULL) perror("fifo0");
        fflush(stdout);
        rc = dup2(fileno(fileIn),fileno(stdin));
        if (rc != 0) perror("stdin");

        if (Arguments::getInstance().resourceFileName().length() > 0)
        {
        	std::string resourceFile = "HERCULES_RC=" + Arguments::getInstance().resourceFileName();
        	int stat = putenv(const_cast<char *>(resourceFile.c_str()));
            if (stat)
            {
             std::cout<<"failed to define environment variable "<< stat << std::endl;
            }
        }
        rc = execlp(hercules.c_str(),hercules.c_str(),"-d","-f",configName.c_str(),"EXTERNALGUI",NULL);
        std::cout << "***************************************************************" << std::endl
				<< "hercules could not be started (" <<  rc << ")" << std::endl
				<< "check that hercules is properly installed and is on the default path " << std::endl
				<< "or that the path specified in Edit/Preferences is correct." << std::endl
				<< "**************************************************************" << std::endl;
        _exit(1);
    }

    mPid = pid;
    printf("pid=:%d\n",pid);
    return pid;
}

void HerculesExecutor::issueCommand(const char * command)
{
    if (mPid == 0)
        return;

    outDebug(2, std::cout << "issue command:" << command << std::endl);
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"%s\n", command);
            fflush(input);
            return;
    }
    else
    {
        outDebug(3, std::cout << "input=" << input << std::endl);
        return;
    }
}

void HerculesExecutor::issueFormattedCommand(const char * format, const char * arg1)
{
    char buffer[strlen(format)+strlen(arg1)+2];
    sprintf(buffer,format,arg1);
    issueCommand(buffer);
}

void HerculesExecutor::issueFormattedCommand(const char * format, int arg1)
{
    char buffer[strlen(format)+64];
    sprintf(buffer,format,arg1);
    issueCommand(buffer);
}


#else  // hFramework
int HerculesExecutor::run(std::string configName, std::string herculesPath)
{
    mProcess=NULL;
    std::string prog = herculesPath;
    prog += "/hercules";
    QString program = prog.c_str();
    QStringList arguments;
    char comm[256];
    strncpy(comm,program.toAscii().data(),255);
    arguments << "-d" << "-f" << configName.c_str() << "EXTERNALGUI" ;
    mProcess = new QProcess();
    mProcess->start(program,arguments);
    int iter=2;
    while(mProcess->state() == QProcess::NotRunning && --iter > 0)
    {
        sleep(1);
    }
    if (mProcess->state() != QProcess::Running && mProcess->state() != QProcess::Starting)
        return -1;
    Q_PID pid = mProcess->pid();
    if (pid != 0)
        return 0;
    else
        return -1;
}

void HerculesExecutor::issueCommand(const char * command)
{
    mProcess->write(command);
}

void HerculesExecutor::issueFormattedCommand(const char * format, const char * arg1)
{
	std::string buffer;
	buffer.resize(strlen(format)+strlen(arg1)+2);
    sprintf(&buffer[0],format,arg1);
    mProcess->write(buffer.c_str());
}

void HerculesExecutor::issueFormattedCommand(const char * format, int arg1)
{
	std::string buffer;
    buffer.resize(strlen(format)+64);
    sprintf(&buffer[0],format,arg1);
    mProcess->write(buffer.c_str());
}

bool HerculesExecutor::getLine(char * buff, int max)
{
    bool ready=false;
    while (!ready)
    {
        ready = mProcess->waitForReadyRead(3000);
        if (ready)
        {
            hOutDebug(0,"waiting for log");
            QByteArray output = mProcess->readLine(max);
            int len = output.length();
            hOutDebug(0,"log len=" << len << ":" << output.data());
            if (len > 0)
            {
                strncpy(buff, output.data(), (len>max? max : len));
                buff[len] = '\0';
            }
            else
            {
                ready=false;
            }
        }
    }
    return true;
}

bool HerculesExecutor::getStatusLine(char * buff, int max)
{
    bool ready=false;
    while (!ready)
    {
        ready = mProcess->waitForReadyRead(3000);
        if (ready)
        {
            hOutDebug(0,"waiting for status");
            QByteArray output = mProcess->readAllStandardError();
            int len = output.length();
            hOutDebug(0,"status len=" << len);
            if (len > 0)
            {
                strncpy(buff, output.data(), (len>max? max : len));
                buff[len] = '\0';
            }
            else
            {
                ready=false;
            }
        }
    }
    return true;
}
#endif
