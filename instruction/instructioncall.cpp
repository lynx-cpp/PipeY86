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

InstructionCall :: InstructionCall(const std::string& m_instructionCode,int address):InstructionPrivate(address)
{
}

InstructionCall :: ~InstructionCall()
{
}

void InstructionCall :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size() < 10) {
        stat = INS;
        std::cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    valC = readHexSmallEndian(m_instructionCode,2,9);
    icode = 8;
    ifun = 0;
    srcA = NO_REG;
    srcB = ESP;
    dstE = ESP;
    dstM = NO_REG;
}

bool InstructionCall :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    currentOperation =  "R[%esp] <- valB;";
    return readReg(ESP,valB);
}

void InstructionCall :: executeStage()
{
    InstructionPrivate :: executeStage();
    currentOperation = "valE <- valB + (-4);";
    valE = valB + (-4);
    writeForwardReg(ESP,valE,true);
    returnAddr = valP;
    valP = findInstructionFromAddr(valC);
    std :: cerr << "call :  PC <- " << valP << std::endl;
    std :: cerr << "call :  PC <- " << int2Hex(valC) << std::endl;
}

void InstructionCall :: memoryStage()
{
    InstructionPrivate :: memoryStage();
    currentOperation = "valE <- valP";
    m_pipeline->write32BitMemory(valE,findAddrFromInstruction(returnAddr));
}

void InstructionCall :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();
    currentOperation = "R[%esp] <- valE;";
    writeRealReg(ESP,valE);

}
