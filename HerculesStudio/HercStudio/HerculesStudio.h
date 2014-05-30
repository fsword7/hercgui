/*
 *  File:       HerculesStudio.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id: HerculesStudio.h 170 2012-12-17 07:45:20Z dekeljacob $
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
#include <QSharedPointer>
#ifndef HS_BUILD_ID
#define HS_BUILD_ID "Unknown"
#endif
#define HERCSTUDIO_VERSION "Version: 1.5.0"
#define HERCSTUDIO_REVISION "$Revision$"
#define hDEBUG 0
#define UNISTD_INC
#ifdef  Q_WS_WIN
#define hFramework
#define hCRLF
#undef UNISTD_INC
#endif
#include <iostream>

#define hOutDebugNl(level,s)  {if ((level) <= hDEBUG ) \
                    {std::cout << s;} \
                    else {}}
#define hOutDebug(level,s)  hOutDebugNl(level, s << std::endl)

#define checkedConnect(button,slot) \
   if ((button) != NULL ) { connect(button, SIGNAL(clicked()), this , SLOT(slot)) ;} else{};

#define mk_shared_ptr(p) class p ; typedef QSharedPointer<p> p##Ptr;
#define mk_shared_constptr(p) class p ; typedef QSharedPointer<const p> p##ConstPtr;

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
