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
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "y86pipeline.h"
#include "instruction.h"
#include "utility.h"
#define PIPELINE

static Memory oldMemory;
/*
 * Instruction* newNullOp() // create NOP instruction
 * {
 *    prog.push_back(Instruction());
 *    return (&prog[prog.size() - 1]);
 * }
 * 
 * Instruction* getPrediction(InstructionPtr ins) //get new instruction safely
 * {
 *    Instruction* next = ins->prediction();
 *    if (next==NULL) 
 *        return newNullOp();
 *    return next;
 * }
 */

static inline bool safeDelete(Instruction* ins)
{
    if (ins==NULL) return false;
    delete ins;
    return true;
}

void nextStage(InstructionPtr& next,InstructionPtr now)
{
    if (now->isBubble()){
        next = (new Instruction(now->addr()));
        delete now;
        return ;
    }
    next = now;
}

Instruction* getPrediction(Instruction* ins)
{
    int idx = ins->prediction();
    if (idx<0 || idx>=prog.size()) 
        return (new Instruction());
    return (new Instruction(prog[idx]));
}

void Y86Pipeline::execute()
{
    #ifndef PIPELINE
    fetchI.fetchStage();
    fetchI.decodeStage();
    fetchI.memoryStage();
    fetchI.executeStage();
    fetchI.writeBackStage();
    fetchI = nextPrediction;
    return ;
    #else
    //std :: cerr << " Done 1" << std :: endl;
    decodeI->setOk();
    
    writeBackI->writeBackStage();
    memoryI->memoryStage();
    executeI->executeStage();
    bool success = decodeI->decodeStage();
    fetchI->fetchStage();
    //std :: cerr << " Done 2" << std :: endl;
    if (!success)
        decodeI->setBubble();
    
    InstructionPtr nextPrediction = getPrediction(fetchI);
    
    InstructionPtr executePrediction = getPrediction(executeI);
    if (executeI->isOk())
        if (!executePrediction->eq(decodeI)){
            decodeI->setBubble();
            fetchI->setBubble();
            delete nextPrediction;
            nextPrediction = executePrediction;
        }
    //std :: cerr << " Done 3" << std :: endl;
    InstructionPtr memoryPrediction = getPrediction(memoryI);
    if (memoryI->isOk())
        if (!memoryPrediction->eq(executeI)){
            fetchI->setBubble();
            decodeI->setBubble();
            executeI->setBubble();
            delete nextPrediction;
            nextPrediction = memoryPrediction;
        }
        //prediction may change.
    //std :: cerr << " Done 4" << std :: endl;
        
    delete writeBackI;
    nextStage(writeBackI,memoryI);
    nextStage(memoryI,executeI);
    if (success){
        nextStage(executeI,decodeI);
        nextStage(decodeI,fetchI);
        fetchI = nextPrediction;
    }
    else 
        executeI = new Instruction(decodeI->addr());
    /*
    writeBackI = memoryI;
    memoryI = executeI;
    executeI = decodeI;
    decodeI = fetchI;
    fetchI = nextPrediction;*/
    
    //recoverForwarding();
    //std::cerr << executeI->prediction() << std::endl;
    #endif
}

Y86Pipeline::Y86Pipeline()
{
    m_loaded = false;
    writeBackI = new Instruction();
    memoryI = new Instruction();
    executeI = new Instruction();
    decodeI = new Instruction();
    fetchI = new Instruction();
    std::cerr << "Warning : empty PIPELINE created." << std::endl;
}

Y86Pipeline::Y86Pipeline(const std::string& filename)
{
    m_loaded = false;
    writeBackI = new Instruction();
    memoryI = new Instruction();
    executeI = new Instruction();
    decodeI = new Instruction();
    fetchI = new Instruction();
    prog.clear();
    m_memory.clear();
    orgStackAddr = 0;
    std::fstream stream;
    std::cerr << "Opening " << filename << std::endl;
    stream.open(filename.c_str(),std::fstream::in);
    if (!stream.is_open()){
        std::cerr << "Failed opening file " << filename << std::endl;
        return ;
    }
    
    while (!stream.eof()){
        std::string s1,s2,s3;
        readTripleStr(stream,s1,s2,s3);
        if (s1=="|" || s1=="")
            continue;
        if (s2=="" || s2=="|")            
            continue;
        
        s1.erase(s1.end() - 1);
        int curAddr = readHexBigEndian(s1,2,s1.size() - 1);
        if (curAddr>orgStackAddr)
            orgStackAddr = curAddr;
        
        if (s2!="")
            prog.push_back(Instruction(s2,s3,curAddr));
        //std::cerr << s3 << std::endl;
        
        for (int i=0;i+1<s2.size();i+=2){
            m_memory[curAddr] = byte2int(s2[i],s2[i + 1]);
            curAddr ++;
        }
    }
    oldMemory = m_memory;
    
    /*
     *    InstructionPtr it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); writeBackI = it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); memoryI = it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); executeI = it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); decodeI= it;
     */
    memset(m_register,0,sizeof(m_register));
    m_register[ESP] = orgStackAddr + 4;
    m_register[EBP] = orgStackAddr;
    recoverForwarding();
    setProgToThis();
    
    delete fetchI;
    fetchI = new Instruction(prog[0]);
    
    m_loaded = true;
    std::cerr << "initialization completed." << std::endl;
}

void Y86Pipeline::setConditionCode(int a, int b, int val)
{
    OverflowFlag = (a<0 == b<0) && (val<0 != a<0);
    ZeroFlag = (val == 0);
    SignFlag = (val<0);
}

void Y86Pipeline::run()
{
    setProgToThis();
   std::cerr<< "starting Y86Pipeline.." << std::endl;
    do{
        std :: cerr << "line start" << std::endl;
        execute();
		for (std::map<int,int>::iterator it=m_memory.begin(); it!=m_memory.end(); ++it)
            std :: cout << int2Hex(it->first)	 << " : " << int2Hex(it->second) << std::endl;
		std :: cout << std::endl;
        std :: cerr << "line end" << std::endl;
    } while (running());
    for (int i=0;i<8;i++){
        if (m_register[i]!=0)
            std::cout << "Value of Register " << i << " changed from 0 to " << m_register[i] << std::endl;
    }
}

bool Y86Pipeline::running()
{
    if (fetchI->normal() || decodeI->normal() || executeI->normal() 
        || memoryI->normal() || writeBackI->normal())
        return true;
    
    return (fetchI->prediction()!=-1);
}

void Y86Pipeline::recoverForwarding()
{
    for (int i=0;i<10;i++)
        forwardReg[i] = m_register[i];
    for (int i=0;i<10;i++)
        forwardStat[i] = true;
}

int Y86Pipeline::read32BitMemory(int address)
{
    int ans = 0;
    for (int i=address + 3;i>=address;i--)
        ans = ans*0x100 + m_memory[i];
	std :: cerr << "reading " << ans << " from M[" << address << "]" << std::endl;
	return ans;
}

void Y86Pipeline::write32BitMemory(int address, int value)
{
	std :: cerr << "writting " << value << " to M[" << address << "]" << std::endl;
    for (int i=address ;i<address+4;i++){
        m_memory[i] = value % 0x100;
        value /= 0x100;
    }
}
Y86Pipeline::~Y86Pipeline()
{
    //delete fetchI; delete decodeI; delete executeI; delete memoryI; delete writeBackI;
    safeDelete(fetchI); 
    safeDelete(decodeI); 
    safeDelete(executeI); 
    safeDelete(memoryI); 
    safeDelete(writeBackI);
}

void Y86Pipeline::setProgToThis()
{   
    for (int i=0;i<prog.size();i++){
        prog[i].setPipeline(this);
        //prog[i].printCode();
    }
}

Y86Pipeline::Y86Pipeline(const Y86Pipeline& org):
m_memory(org.m_memory),
orgStackAddr(org.orgStackAddr),
ZeroFlag(org.ZeroFlag),
OverflowFlag(org.OverflowFlag),
SignFlag(org.SignFlag),
m_loaded(org.m_loaded)
{
    fetchI = new Instruction(*org.fetchI);
    decodeI = new Instruction(*org.decodeI);
    executeI = new Instruction(*org.executeI);
    memoryI = new Instruction(*org.memoryI);
    writeBackI = new Instruction(*org.writeBackI);
    
    memcpy(m_register,org.m_register,sizeof(m_register));
    memcpy(forwardReg,org.forwardReg,sizeof(forwardReg));
    memcpy(forwardStat,org.forwardStat,sizeof(forwardStat));
}
