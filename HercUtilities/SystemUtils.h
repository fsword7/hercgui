/*
 * SystemUtils.h
 *
 *  Created on: Aug 24, 2009
 *      Author: yakovd
 *
 *  $Id: SystemUtils.h 34 2009-11-07 06:15:58Z jacob $
 */

#ifndef SYSTEMUTILS_H_
#define SYSTEMUTILS_H_

#include <string>

class SystemUtils
{
public:
	SystemUtils();
	virtual ~SystemUtils();

	static bool processIsRunning(int pid);
	static bool fileExists(const std::string& filename);
};

#endif /* SYSTEMUTILS_H_ */
