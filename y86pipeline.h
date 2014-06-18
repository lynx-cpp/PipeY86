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

#ifndef Y86PIPELINE_H
#define Y86PIPELINE_H
#include <cstdio>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

class Instruction;
class PipelineLoader;
#include "instruction.h"
typedef std::vector<Instruction> Program;
typedef Instruction* InstructionPtr;
typedef std::map<int,int> Memory;

#define MAX_REG_NUM 16

class Y86Pipeline
{
private:
    status stat;
    
    bool m_loaded;
    
    InstructionPtr fetchI,decodeI,executeI,memoryI,writeBackI;
    int m_register[MAX_REG_NUM];
    
    int forwardReg[MAX_REG_NUM];
    bool forwardStat[MAX_REG_NUM];
    
    Memory m_memory;
    int orgStackAddr;
    
    bool ZeroFlag,OverflowFlag,SignFlag;
    
    void writeForwarding(int num,int value,bool stat)
    {
        forwardReg[num] = value;
        forwardStat[num] = stat;
    }
    bool readForwarding(int num,int& dest)
    {
        if (!forwardStat[num])
            return false;
        dest = forwardReg[num];
        return true;
    }
    void writeRegister(int num,int value) 
    { 
        if (num>=MAX_REG_NUM)
            stat = ADR;
        m_register[num] = value; 
        std::cerr << "writing value " << value << " to R[" << num << "]" << std::endl;
    }
    int readMemory(int address) { return m_memory[address]; }
    void writeMemory(int address,int value)
	{
		m_memory[address] = value; 
		std :: cerr << "writing value " << value << " to M[" << address << "]" << std::endl;
	}
   void recoverForwarding();
    
public:
    friend class InstructionPrivate;
    friend class PipelineLoader;
    Y86Pipeline(const std::string& filename);
    Y86Pipeline(const Y86Pipeline& org);
    explicit Y86Pipeline();
    ~Y86Pipeline();
    void setProgToThis();
    bool running();
    void run();
    void execute();
    //int readRegister(int num) { return m_register[num]; }
    
    bool loaded() { return m_loaded; }
    
    int read32BitMemory(int address) ;
    void write32BitMemory(int address,int value);
    void setConditionCode(int a,int b,int val);
    bool jle() { return (SignFlag ^ OverflowFlag) || ZeroFlag; }
    bool jl() { return SignFlag ^ OverflowFlag; }
    bool je() { return ZeroFlag; }
    bool jne() { return !ZeroFlag; }
    bool jge() { return !(SignFlag ^ OverflowFlag); }
    bool jg() { return !(SignFlag ^ OverflowFlag) && (!ZeroFlag); }
    
    void printStageStatus(std::fstream& stream);
    //int nextInstruction(int current,int step = 1) {  }
};

#endif // Y86PIPELINE_H
