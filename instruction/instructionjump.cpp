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

InstructionJump :: InstructionJump(const std::string& instructionCode, int address): InstructionPrivate(address)
{
    icode = 7;
    ifun = hex2num(instructionCode[1]);
    switch (ifun)
    {
    case 0:
        type = jmp;
        jumpString = "jmp";
        break;
    case 1:
        type = jle;
        jumpString = "jle";
        break;
    case 2:
        type = jl;
        jumpString = "jl";
        break;
    case 3:
        type = je;
        jumpString = "je";
        break;
    case 4:
        type = jne;
        jumpString = "jne";
        break;
    case 5:
        type = jge;
        jumpString = "jge";
        break;
    case 6:
        type = jg;
        jumpString = "jg";
        break;
    default:
        type = other;
        jumpString = "other";
        break;
    }
}

InstructionJump :: ~InstructionJump()
{
}

void InstructionJump :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size() < 10) {
        stat = INS;
        std :: cerr << "invalid instruction." << std :: endl;
        //invalid instruction ...
    }
    valC = readHexSmallEndian(m_instructionCode,2,9);
    currentOperation = "valC <- M_4[PC];";
    srcA = 8;
    srcB = 8;
    dstE = 8;
    dstM = 8;
}

bool InstructionJump :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    return true;
    //???
}

void InstructionJump :: executeStage()
{
    InstructionPrivate :: executeStage();
    switch (type)
    {
    case jmp:
        BCH = true;
        break;
    case jl:
        BCH = m_pipeline->jl();
        break;
    case jle:
        BCH = m_pipeline->jle();
        break;
    case je:
        BCH = m_pipeline->je();
        break;
    case jne:
        BCH = m_pipeline->jne();
        break;
    case jge:
        BCH = m_pipeline->jge();
        break;
    case jg:
        BCH = m_pipeline->jg();
        break;
    default:
        std :: cerr << jumpString << std::endl;
        break;
    }
    currentOperation = "Bch <- Cond(CC,ifun);";
    if (BCH)
    {
        valP = findInstructionFromAddr(valC);
        currentOperation +=" valP <- valC;";
    }
}

void InstructionJump :: memoryStage()
{
    InstructionPrivate :: memoryStage();
}

void InstructionJump :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();

}
