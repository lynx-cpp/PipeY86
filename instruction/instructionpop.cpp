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

InstructionPop :: InstructionPop(const std::string& m_instructionCode, int address):InstructionPrivate(address)
{
    icode = 0xb;
    ifun = 0;
}
InstructionPop :: ~InstructionPop()
{
}

void InstructionPop :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size() < 4) {
        stat = INS;
        std :: cerr << "invalid instrucion." << std::endl;
        //invalid instruc ...
    }
    rA = hex2num(m_instructionCode[2]);
    srcA = ESP;
    srcB = ESP;
    dstE = ESP;
    dstM = rA;
}

bool InstructionPop :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    currentOperation = "valA <- R[%esp]; valB <- R[%esp];";
    return (readReg(ESP,valA) && readReg(ESP,valB));
}

void InstructionPop :: executeStage()
{
    InstructionPrivate :: executeStage();
    valE = valB + 4;
    currentOperation = "valE <- valB + 4";
    writeForwardReg(rA,valM,false);
    writeForwardReg(ESP,valE,true);
    //valM = m_pipeline->read32BitMemory(valA);
}

void InstructionPop :: memoryStage()
{
    InstructionPrivate :: memoryStage();
    valM = m_pipeline->read32BitMemory(valA);
    currentOperation = "valM <- M_4[valA];";
	writeForwardReg(ESP,valE,true);
    writeForwardReg(rA,valM,true);
}

void InstructionPop :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();
    writeRealReg(ESP,valE);
    writeRealReg(rA,valM);
    currentOperation = "R[%esp] <- valE; R[rA] <- valM";
}
