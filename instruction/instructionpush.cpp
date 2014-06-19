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

InstructionPush :: InstructionPush(const std::string& instructionCode, int address):InstructionPrivate(address)
{
    codeLength = 2;
    icode = 0xa;
    ifun = 0;
}

InstructionPush :: ~InstructionPush()
{
}

void InstructionPush :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size() < 4) {
        stat = INS;
        std :: cerr << "invalid instrucion." << std::endl;
        //invalid instruc ...
    }
    rA = hex2num(m_instructionCode[2]);
    srcA = rA;
    srcB = ESP;
    dstE = ESP;
}

bool InstructionPush :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    currentOperation = "valA <- R[rA]; valB <- R[%esp];";
    return (readReg(rA,valA) && readReg(ESP,valB));
}

void InstructionPush :: executeStage()
{
    InstructionPrivate :: executeStage();
    valE = valB + (-4);
    currentOperation = "valE <- valB + (-4);";
    writeForwardReg(ESP,valE,true);
}

void InstructionPush :: memoryStage()
{
    InstructionPrivate :: memoryStage();
    m_pipeline->write32BitMemory(valE,valA);
	writeForwardReg(ESP,valE,true);
}

void InstructionPush :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();
    writeRealReg(ESP,valE);
    currentOperation = "R[%esp] <- valE;";
}



