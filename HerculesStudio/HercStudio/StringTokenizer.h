/*
 *  File:       StringTokenizer.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object enables tokenizing a line of text by delimiters
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

#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include <string>
#include <iostream>

namespace Tokenizer
{
    typedef std::string::size_type handle;
}

class StringTokenizer
{
public:
    static inline std::string getFirstWord(const std::string& line, std::string::size_type &pos, std::string::size_type &lastPos, const std::string& delimiters)
    {
        lastPos = line.find_first_not_of(delimiters, 0);
        if (lastPos == std::string::npos) return ("");
        pos = line.find_first_of(delimiters, lastPos);
        if (pos == std::string::npos) return (line);

        return line.substr(lastPos, pos-lastPos);
    }

    static inline std::string getNextWord(const std::string& line, std::string::size_type &pos, std::string::size_type &lastPos, const std::string& delimiters)
    {
        lastPos = line.find_first_not_of(delimiters, pos);
        if (lastPos == std::string::npos) return ("");
        pos = line.find_first_of(delimiters, lastPos);
        if (pos == std::string::npos) pos=line.length();

        return line.substr(lastPos, pos-lastPos);

    }
};
#endif // STRINGTOKENIZER_H
