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

InstructionRmmovl :: InstructionRmmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
    codeLength = 6;
}

InstructionRmmovl :: ~InstructionRmmovl()
{

}

void InstructionRmmovl :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size()<12) {
        stat = INS;
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
    icode = 4;
    ifun = 0;
    srcA = rA;
    srcB = rB;
    dstE = 8;
    dstM = 8;
}

bool InstructionRmmovl :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    currentOperation = "R[rA] <- valA; R[rB] <- valB;";
    return (readReg(rA,valA) && readReg(rB,valB));
}

void InstructionRmmovl :: executeStage()
{
    InstructionPrivate :: executeStage();
    currentOperation = "valE <- valB + valC;";
    valE = valB + valC;
}

void InstructionRmmovl :: memoryStage()
{
    InstructionPrivate :: memoryStage();
    currentOperation = "M_4[valE] <- valA;";
    std :: cerr << valA << std::endl;
    m_pipeline->write32BitMemory(valE,valA);
}

void InstructionRmmovl :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();
}
