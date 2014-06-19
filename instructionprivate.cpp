/*
 *   Copyright (C) 2014 by Yuquan Fang<lynx.cpp@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "instructionprivate.h"
#include "y86pipeline.h"

int findInstructionFromAddr(int address)
{
    int len = prog.size();
    for (int i=0; i<len; i++)
        if (prog[i].addr()==address)
            return i;
    return -1;
}

int findAddrFromInstruction(int idx)
{
    if (idx<0 || idx>=prog.size()) 
        return -1;
    return prog[idx].addr();
}


InstructionPrivate::InstructionPrivate(int address):
    icode(0),ifun(0),rA(NO_REG),rB(NO_REG),valC(0),valA(0),valB(0),valE(0),valM(0),srcA(NO_REG),srcB(NO_REG),dstE(NO_REG),dstM(NO_REG),BCH(false),codeLength(1)
{
    m_address = address;
    stat = BUB;
    valP = -1;
}

bool InstructionPrivate::readReg(int num, int& dest)
{
    if (num>=MAX_REG_NUM){
        stat = ADR;
        return true;
    }
    return m_pipeline->readForwarding(num,dest);
}

void InstructionPrivate::writeForwardReg(int num, int value, bool flag)
{
    m_pipeline->writeForwarding(num,value,flag);
}

void InstructionPrivate::writeRealReg(int num, int value)
{
    m_pipeline->writeForwarding(num,value,true);
    m_pipeline->writeRegister(num,value);
}

void InstructionPrivate::fetchStage()
{
    /*if (m_address==-1) {
        valP = -1;
        return ;
    }*/
    valP = findInstructionFromAddr(m_address);
    if (valP==-1)
        return ;
    valP ++;
    currentOperation = "NOP;";
}


bool InstructionPrivate::decodeStage()
{
    //std::cerr << "next decode: " << valP << " " << int2Hex(findAddrFromInstruction(valP)) << std::endl;
    currentOperation = "NOP;";
    return true;
}

void InstructionPrivate::executeStage()
{
    currentOperation = "NOP;";
}

void InstructionPrivate::memoryStage()
{
    currentOperation = "NOP;";
}

void InstructionPrivate::writeBackStage()
{
    currentOperation = "NOP;";
}


