/*
 * SystemUtils.h
 *
 *  Created on: Aug 24, 2009
 *      Author: yakovd
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
