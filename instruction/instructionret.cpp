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

InstructionRet :: InstructionRet(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
}

InstructionRet :: ~InstructionRet()
{
}

void InstructionRet :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    icode = 9;
    ifun = 0;
    srcA = 4;
    srcB = 4;
    dstE = 8;
    dstM = 4;
}

bool InstructionRet :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    currentOperation = "valA <- R[%esp]; valB <- R[%esp];";
    return (readReg(4,valA) && readReg(4,valB));
}

void InstructionRet :: executeStage()
{
    InstructionPrivate :: executeStage();
    currentOperation = "valE <- valB + 4;";
    valE = valB + 4;
	writeForwardReg(4,valE,true);
}

void InstructionRet :: memoryStage()
{
    InstructionPrivate :: memoryStage();
    currentOperation = "valM <- M_4[valA]";
    valM = m_pipeline->read32BitMemory(valA);
    writeForwardReg(4,valE,true);
    valP = findInstructionFromAddr(valM);
    std :: cerr << "Return : valP <- " << valM << std::endl;
}

void InstructionRet :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();
    writeRealReg(4,valE);
    currentOperation = "R[%esp] <- valE;";
}
