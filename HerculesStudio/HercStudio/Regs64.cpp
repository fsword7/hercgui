/*
 * Regs64.cpp
 *
 *  Created on: Aug 19, 2009
 *      Author: yakovd
 */

#include "Regs64.h"

std::string Regs64::gCommand = "]GREGS64=";
std::string Regs64::cCommand = "]CREGS64=";
std::string Regs64::fCommand = "]FREGS64=";

Regs64::Regs64(QWidget * parent, std::string command) :
	Regs32(parent, command),
	mLine5(this),
	mLine6(this),
	mLine7(this),
	mLine8(this)
{
    QFont font("monospace", 9);
    font.setStyleHint(QFont::Courier);
    mLine5.setFont(font);
    mLine6.setFont(font);
    mLine7.setFont(font);
    mLine8.setFont(font);
    mLine5.setGeometry(340,10,330,12);
    mLine6.setGeometry(340,26,330,12);
    mLine7.setGeometry(340,42,330,12);
    mLine8.setGeometry(340,58,330,12);

}

Regs64::~Regs64()
{
}

void Regs64::setActive(bool active)
{
	Regs32::setActive(active);
    mLine5.setVisible(active);
    mLine6.setVisible(active);
    mLine7.setVisible(active);
    mLine8.setVisible(active);
    std::string command = mCommand + (active ? '1' : '0');
}

void Regs64::notify(const std::string& statusLine)
{
	if (statusLine[5] == '0')
		Regs32::mLine1.setText(statusLine.c_str());
	if (statusLine[5] == '2')
		Regs32::mLine2.setText(statusLine.c_str());
	if (statusLine[5] == '4')
		Regs32::mLine3.setText(statusLine.c_str());
	if (statusLine[5] == '6')
		Regs32::mLine4.setText(statusLine.c_str());
	if (statusLine[5] == '8')
		Regs64::mLine5.setText(statusLine.c_str());
	if (statusLine[5] == 'A')
		Regs64::mLine6.setText(statusLine.c_str());
	if (statusLine[5] == 'C')
		Regs64::mLine7.setText(statusLine.c_str());
	if (statusLine[5] == 'E')
		Regs64::mLine8.setText(statusLine.c_str());
}
