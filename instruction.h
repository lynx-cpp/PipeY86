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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <QVariantList>


class Y86Pipeline;
class Instruction;
class InstructionPrivate;
class InstructionOP;

#include "utility.h"
enum status {
    AOK,HLT,ADR,INS,BUB
} ;

class Instruction
{
private:
    InstructionPrivate* instructionP;
    std::string m_instructionCode,m_comment;
    int m_address;
    void constructPrivate();

public:
    friend class InstructionPrivate;
    Instruction(const std::string& instructionCode,const std::string& comment,int address);
    Instruction(int address = -1);
    Instruction(const Instruction& ip) ;
    ~Instruction();
    void setPipeline(Y86Pipeline* pipeline);
    int prediction() const;
    void printCode();

    void setBubble();
    bool isBubble();
    void setOk();
    bool isOk();
    bool normal();

    void fetchStage();
    bool decodeStage();
    void executeStage();
    void memoryStage();
    void writeBackStage();

    int addr();
    std::string instructionCode();
    std::string comment();
    std::string currentOperation();

    bool operator!=(const Instruction& B);
    bool eq(Instruction* decodeI);
    
    status stat() const;
    
    int d_valP() const; // here is the real valP in the CSAPP book
    
    void printFetchStatus(std::fstream& stream);
    void printDecodeStatus(std::fstream& stream);
    void printExecuteStatus(std::fstream& stream);
    void printMemoryStatus(std::fstream& stream);
    void printWritebackStatus(std::fstream& stream);

#ifdef QT_VERSION
    QVariantList stageRegStatus() const;
#endif
};

#endif
