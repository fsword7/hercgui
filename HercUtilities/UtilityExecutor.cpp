/*
 *  File:       UtilityExecutor.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
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

#include "UtilityExecutor.h"

#include <sys/wait.h>
#include <cerrno>
#include <cstdio>

UtilityExecutor::UtilityExecutor()
{
}

UtilityExecutor::~UtilityExecutor()
{
}

#include "UtilityExecutor.h"

#include <unistd.h>

int UtilityExecutor::run(const std::string & pCommand, const std::string& pPath, std::vector<std::string> pParameters)
{
    int pid;
    int rc =0;

    rc = pipe(mPipeError);
    if (rc)
    {
        printf("pipe() rc=%d\n",rc);
        return 0;
    }
    rc = pipe(mPipeOut);
    if (rc)
    {
        printf("pipe() rc=%d\n",rc);
        return 0;
    }

    rc = pipe(mPipeIn);
    if (rc)
    {
        printf("pipe() rc=%d\n",rc);
        return 0;
    }

    pid = fork();
    if (pid < 0)
    {
        return -1;
    }
    if (pid == 0)
    {
        std::string command = pPath;
        if (command.length() != 0 )
            command += "/";
        command += pCommand;
        outDebug(5, std::cout << "command=" << command << std::endl);
        close(mPipeOut[0]);
        close(mPipeIn[1]);
        close(mPipeError[0]);
        rc = dup2(mPipeError[1],fileno(stderr));close(mPipeError[1]);
        rc = dup2(mPipeOut[1],fileno(stdout)); close(mPipeOut[1]);
        rc = dup2(mPipeIn[0],fileno(stdin)); close(mPipeIn[0]);

        getchar();

        char * pVec[pParameters.size()+2];
        std::vector<std::string>::iterator first, end;
        first = pParameters.begin();
        end = pParameters.end();
        const char **p = const_cast<const char **> (pVec);
        *p++ = command.c_str();
        std::cerr << pCommand << " ";
        while (first != end)
        {
            if (*first != "EXTERNALGUI")
            {
                std::cerr <<  *first << " ";
            }
            *p++ = first++->c_str();
        }
        std::cerr << std::endl;

        *p=NULL;
        execvp(command.c_str(), pVec);
        abort();
    }

    close(mPipeOut[1]);
    close(mPipeIn[0]);
    close(mPipeError[1]);

    printf("pid:=%d\n",pid);
    return pid;
}

int UtilityExecutor::getPipeIn()
{
    return mPipeIn[1];
}

int UtilityExecutor::getPipeOut()
{
    return mPipeOut[0];
}

int UtilityExecutor::getPipeError()
{
    return mPipeError[0];
}

bool UtilityExecutor::evaluateProcess(int pid)
{
    int status;
    int reply = waitpid(pid, &status, WNOHANG);

    outDebug(2, std::cout << pid << ": evaluate reply=" << reply << " errno=" << errno);
    if (WIFEXITED(status))
    {
        outDebug(2, std::cout << " exited. status=" << WEXITSTATUS(status) << std::endl);
        return true;

    }
    else
    {
        outDebug(2, std::cout << "running" << std::endl);
    }
    return false;

}
