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

InstructionMrmovl::InstructionMrmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
    codeLength = 6;
}

InstructionMrmovl::~InstructionMrmovl()
{
}

void InstructionMrmovl::fetchStage()
{
    InstructionPrivate::fetchStage();
    if (m_instructionCode.size()<12) {
        stat = INS;
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
    icode = 5;
    ifun = 0;
    srcA = 8;
    srcB = rB;
    dstE = 8;
    dstM = rA;
}

bool InstructionMrmovl::decodeStage()
{
    InstructionPrivate :: decodeStage();
    currentOperation = "R[rB] <- valB;";
    return readReg(rB,valB);
}

void InstructionMrmovl::executeStage()
{
    InstructionPrivate::executeStage();
    valE = valB + valC;
    writeForwardReg(rA,valM,false);
    currentOperation = "valE <- valB + valC;";
}

void InstructionMrmovl::memoryStage()
{
    InstructionPrivate::memoryStage();
    valM = m_pipeline->read32BitMemory(valE);
    std :: cerr << "Check " << valM << std::endl;
    writeForwardReg(rA,valM,true);
    currentOperation = "valM <- M_4[valE];";
}

void InstructionMrmovl::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    writeRealReg(rA,valM);
    currentOperation = "R[rA] <- valM;";
}
