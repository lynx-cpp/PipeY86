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

#include <iostream>
#include "instruction.h"
#include "y86pipeline.h"
#include "utility.h"
#include "instructionprivate.h"

void Instruction::constructPrivate()
{
    instructionP = NULL;
    if (m_instructionCode.length()<2) {
        std::cerr << "instruction code \'"<< m_instructionCode <<"\' length too short" << std::endl;
        instructionP = new InstructionNop(m_address);
        instructionP->m_instructionCode = m_instructionCode;
        return ;
    }
    int code = byte2int(m_instructionCode[0],m_instructionCode[1]);
    int icode = hex2num(m_instructionCode[0]);
    int ifun = hex2num(m_instructionCode[1]);
    do {
        if (icode==6) {
            instructionP = new InstructionOP(m_instructionCode,m_address);
            break;
        }
        if (code==0x10) {
            instructionP = new InstructionHalt(m_instructionCode,m_address);
            break;
        }
        if (code==0x20) {
            instructionP = new InstructionRrmovl(m_instructionCode,m_address);
            break;
        }
        if (code==0x30) {
            instructionP = new InstructionIrmovl(m_instructionCode,m_address);
            break;
        }
        if (code==0x40) {
            instructionP = new InstructionRmmovl(m_instructionCode,m_address);
            break;
        }
        if (code==0x50) {
            instructionP = new InstructionMrmovl(m_instructionCode,m_address);
            break;
        }
        if (icode==0x7) {
            instructionP = new InstructionJump(m_instructionCode,m_address);
            break;
        }
        if (code==0x80) {
            instructionP = new InstructionCall(m_instructionCode,m_address);
            break;
        }
        if (code==0x90) {
            instructionP = new InstructionRet(m_instructionCode,m_address);
            break;
        }
        if (code==0xa0) {
            instructionP = new InstructionPush(m_instructionCode,m_address);
            break;
        }
        if (code==0xb0) {
            instructionP = new InstructionPop(m_instructionCode,m_address);
            break;
        }
        //add new instruction constructing function here

        instructionP = new InstructionNop(m_address);
        if (m_instructionCode!="00" && m_instructionCode!="")
            instructionP->stat = INS;
    } while (false);
    instructionP->m_instructionCode = m_instructionCode;
}

Instruction::Instruction(const Instruction& ip)
{
    m_instructionCode = ip.m_instructionCode;
    if (m_instructionCode=="")
        m_instructionCode = "00";
    m_address = ip.m_address;
    m_comment = ip.m_comment;
    constructPrivate();
    instructionP->setPipeline(ip.instructionP->m_pipeline);
    instructionP->stat = ip.instructionP->stat;
    instructionP->rA = ip.instructionP->rA;
    instructionP->rB = ip.instructionP->rB;
    instructionP->valA = ip.instructionP->valA;
    instructionP->valB = ip.instructionP->valB;
    instructionP->valC = ip.instructionP->valC;
    instructionP->valE = ip.instructionP->valE;
    instructionP->valM = ip.instructionP->valM;
    instructionP->valP = ip.instructionP->valP;
}

Instruction::Instruction(const std::string& instructionCode, const std::string& comment, int address):m_comment(comment)
{
    m_instructionCode = instructionCode;
    m_address = address;
    constructPrivate();
}

Instruction::Instruction(int address)
{
    m_instructionCode = "00";
    m_address = address;
    constructPrivate();
    m_instructionCode = "";
}

Instruction::~Instruction()
{
    if (instructionP!=NULL)
        delete instructionP;
    else
        std::cerr << "InstructionPrivate Error.." << std::endl;
}

void Instruction::setPipeline(Y86Pipeline* pipeline)
{
    instructionP->setPipeline(pipeline);
}

int Instruction::prediction() const
{
    return instructionP->prediction();
}

void Instruction::printCode()
{
    std::cerr << "code : " << m_instructionCode << std::endl;
}

void Instruction::setBubble()
{
    instructionP->stat = BUB;
}

bool Instruction::isBubble()
{
    return (instructionP->stat == BUB);
}

void Instruction::setOk()
{
    instructionP->stat = AOK;
}

bool Instruction::isOk()
{
    return (instructionP->stat==AOK);
}

bool Instruction::normal()
{
    return (isOk() || instructionP->stat==HLT) && (m_address!=-1);
}

void Instruction::fetchStage()
{
    if (instructionP->stat==BUB) {
        instructionP->stat = AOK;
        instructionP->fetchStage();
    }
}

bool Instruction::decodeStage()
{
    if (instructionP->stat==AOK) return instructionP->decodeStage();
    return true;
}

void Instruction::executeStage()
{
    if (instructionP->stat==AOK) instructionP->executeStage();
}

void Instruction::memoryStage()
{
    if (instructionP->stat==AOK) instructionP->memoryStage();
}

void Instruction::writeBackStage()
{
    if (instructionP->stat==AOK) instructionP->writeBackStage();
}

int Instruction::addr()
{
    return instructionP->addr();
}

bool Instruction::operator!=(const Instruction& B)
{
    return m_address!=B.m_address;
}

bool Instruction::eq(Instruction* decodeI)
{
    if (addr()==-1)
        return true;
    return addr()==decodeI->addr();
}

std::string Instruction::instructionCode()
{
    return m_instructionCode;
}

std::string Instruction::comment()
{
    return m_comment;
}

std::string Instruction::currentOperation()
{
    return instructionP->currentOperation;
}

status Instruction::stat() const
{
    return instructionP->stat;
}

void Instruction::printDecodeStatus(std::fstream& stream)
{
    stream << "DECODE:" << std::endl;
    stream << "\tD_icode  \t= " << int2Hex(instructionP->icode,1) << std::endl;
    stream << "\tD_ifun   \t= "  << int2Hex(instructionP->ifun,1) << std::endl;
    stream << "\tD_rA     \t= "  << int2Hex(instructionP->rA,1) << std::endl;
    stream << "\tD_rB     \t= " << int2Hex(instructionP->rB,1) << std::endl;
    stream << "\tD_valC   \t= " << int2Hex(instructionP->valC,8) << std::endl;
    stream << "\tD_valP   \t= " << int2Hex(instructionP->addr(),8) << std::endl;
    stream << std::endl;
}

void Instruction::printExecuteStatus(std::fstream& stream)
{
    stream << "EXECUTE:" << std::endl;
    stream << "\tE_icode  \t= " << int2Hex(instructionP->icode,1) << std::endl;
    stream << "\tE_ifun    \t= "  << int2Hex(instructionP->ifun,1) << std::endl;
    stream << "\tE_valC    \t= " << int2Hex(instructionP->valC,8) << std::endl;
    stream << "\tE_valA    \t= " << int2Hex(instructionP->valA,8) << std::endl;
    stream << "\tE_valB    \t= " << int2Hex(instructionP->valB,8) << std::endl;
    stream << "\tE_dstE    \t= "  << int2Hex(instructionP->dstE,1) << std::endl;
    stream << "\tE_dstM   \t= "  << int2Hex(instructionP->dstM,1) << std::endl;
    stream << "\tE_srcA    \t= "  << int2Hex(instructionP->srcA,1) << std::endl;
    stream << "\tE_srcB    \t= " << int2Hex(instructionP->srcB,1) << std::endl;
    stream << std::endl;
}

void Instruction::printFetchStatus(std::fstream& stream)
{
    stream << "FETCH:" << std::endl;
    stream << "\tF_predPC \t= " << int2Hex(addr(),8) << std::endl << std::endl;
}

void Instruction::printMemoryStatus(std::fstream& stream)
{
    std::string bch = "true";
    if (!instructionP->BCH)
        bch = "false";
    stream << "MEMORY:" << std::endl;
    stream << "\tM_icode  \t= " << int2Hex(instructionP->icode,1) << std::endl;
    stream << "\tM_Bch    \t= "  << bch << std::endl;
    stream << "\tM_valE   \t= " << int2Hex(instructionP->valE,8) << std::endl;
    stream << "\tM_valA   \t= " << int2Hex(instructionP->valA,8) << std::endl;
    stream << "\tM_dstE   \t= "  << int2Hex(instructionP->dstE,1) << std::endl;
    stream << "\tM_dstM   \t= "  << int2Hex(instructionP->dstM,1) << std::endl;
    stream << std::endl;
}

void Instruction::printWritebackStatus(std::fstream& stream)
{
    stream << "WRITE BACK:" << std::endl;
    stream << "\tW_icode  \t= " << int2Hex(instructionP->icode,1) << std::endl;
    stream << "\tW_valE   \t= " << int2Hex(instructionP->valE,8) << std::endl;
    stream << "\tW_valM   \t= " << int2Hex(instructionP->valM,8) << std::endl;
    stream << "\tW_dstE   \t= "  << int2Hex(instructionP->dstE,1) << std::endl;
    stream << "\tW_dstM   \t= "  << int2Hex(instructionP->dstM,1) << std::endl;
    stream << std::endl;
}


#ifdef QT_VERSION
QVariantList Instruction::stageRegStatus() const
{
    int addr = instructionP->addr();
    if (addr==-1) addr = 0;
    QVariantList ret;
    ret.clear();
    ret.append(instructionP->icode);
    ret.append(instructionP->ifun);
    ret.append(instructionP->rA);
    ret.append(instructionP->rB);
    ret.append(instructionP->dstE);
    ret.append(instructionP->dstM);
    ret.append(instructionP->srcA);
    ret.append(instructionP->srcB);
    ret.append(QString::fromStdString(int2Hex(instructionP->valA,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valB,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valC,8)));
    ret.append(QString::fromStdString(int2Hex(addr,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valE,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valM,8)));
    return ret;
}
#endif //QT_VERSION
