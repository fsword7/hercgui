/*
 *  File:       HerculesStudio.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *  a collection of product-wide definitions
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

#ifndef HERCULESSTUDIO_H_
#define HERCULESSTUDIO_H_

#include <qglobal.h>
#define HERCSTUDIO_VERSION "Version: 1.2.0"
#define hDEBUG 0
#ifdef  Q_WS_WIN
#define hFramework
#define hCRLF
#endif
//#define hFramework
#include <iostream>

#define outDebug(level,s) if ((level) <= hDEBUG ) \
					{s;} \
					else {}
#define hOutDebug(level,s)  if ((level) <= hDEBUG ) \
					{std::cout << s << std::endl;} \
					else {}

#define checkedConnect(button,slot) \
   if ((button) != NULL ) { connect(button, SIGNAL(clicked()), this , SLOT(slot)) ;} else{};

class hsException : public  std::exception
{
public:
   hsException(std::string msg)
   {
	   mWhat = msg.c_str();
   }
   virtual ~hsException() throw()
   {

   }
   virtual const char* what() const throw()
   {
	   return mWhat.c_str();
   }
private:
   std::string mWhat;
};

#endif /* HERCULESSTUDIO_H_ */
