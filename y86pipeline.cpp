#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "y86pipeline.h"
#include "instruction.h"
#include "utility.h"
#define PIPELINE

static int* mem_bak;

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

Instruction* getPrediction(Instruction* ins)
{
    int idx = ins->prediction();
    if (idx==-1 || idx>=prog.size()) 
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
    
    writeBackI->writeBackStage();
    memoryI->memoryStage();
    executeI->executeStage();
    decodeI->decodeStage();
    fetchI->fetchStage();
    
    InstructionPtr nextPrediction = getPrediction(fetchI);
    
    InstructionPtr curPrediction = getPrediction(executeI);
    if (executeI->isOk())
        if (curPrediction!=decodeI){
            decodeI->setBubble();
            fetchI->setBubble();
            nextPrediction = curPrediction;
        }
        
    curPrediction  = getPrediction(memoryI);
    if (memoryI->isOk())
        if (curPrediction!=executeI){
            fetchI->setBubble();
            decodeI->setBubble();
            executeI->setBubble();
            nextPrediction = curPrediction;
        }
        //prediction may change.
        
    delete writeBackI;
    writeBackI = memoryI;
    memoryI = executeI;
    executeI = decodeI;
    decodeI = fetchI;
    fetchI = nextPrediction;
    
    #endif
}

Y86Pipeline::Y86Pipeline(const std::string& filename)
{
    std::fstream stream;
    stream.open(filename.c_str(),std::fstream::in);
    startAddr = -1;
    endAddr = startAddr;
    while (!stream.eof()){
        std::string s1,s2;
        readAddrAndValue(stream,s1,s2);
        if (s1=="|" || s1=="")
            continue;
        
        s1.erase(s1.end() - 1);
        int curAddr = readHexBigEndian(s1,2,s1.size() - 1);
        
        if (startAddr==-1)
            startAddr = curAddr;
        endAddr = curAddr;
        //std::cout << s1 << " {} " << s2 << std::endl;
    }
    endAddr += 8;
    //std::cout<< startAddr << " " << endAddr << std::endl;
    m_memory = new char[endAddr - startAddr];
    //mem_bak = static_cast<int*>(std::malloc(endAddr - startAddr));
    //memcpy(mem_bak,m_memory,sizeof(m_memory));
    
    prog.clear();
    
    //stream.seekg(0,stream.beg);
    //stream.seekp(stream.beg);
    stream.close();
    stream.open(filename.c_str(),std::fstream::in);
    while (!stream.eof()){
        std::string s1,s2;
        readAddrAndValue(stream,s1,s2);
        if (s1=="|" || s1=="")
            continue;
        if (s2=="" || s2=="|")            
            continue;
        
        s1.erase(s1.end() - 1);
        int curAddr = readHexBigEndian(s1,2,s1.size() - 1);
        
        prog.push_back(Instruction(s2,curAddr));
        
        for (int i=0;i+1<s2.size();i+=2){
            m_memory[curAddr - startAddr] = byte2int(s2[i],s2[i + 1]);
            curAddr ++;
        }
    }
    
    /*
     *    InstructionPtr it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); writeBackI = it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); memoryI = it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); executeI = it;
     *    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); decodeI= it;
     */
    memset(m_register,0,sizeof(m_register));
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
    /*
     * for (InstructionPtr i=prog.begin();i!=prog.end();i++){
     *    i->setPipeline(this);
     *    i->printCode();
}*/
    int len = prog.size();
    for (int i=0;i<len;i++){
        prog[i].setPipeline(this);
        prog[i].printCode();
    }
    writeBackI = new Instruction();
    memoryI = new Instruction();
    executeI = new Instruction();
    decodeI = new Instruction();
    fetchI = new Instruction(prog[0]);
    std::cerr<< "starting Y86Pipeline.." << std::endl;
    do{
        //InstructionPtr next = getPrediction(fetchI);
        //std::cerr << next->instructionP << std::endl;
        execute();
    } while (running());
    for (int i=0;i<8;i++){
        if (m_register[i]!=0)
            std::cout << i << " " << m_register[i] << std::endl;
    }
}

bool Y86Pipeline::running()
{
    if (fetchI->normal() || decodeI->normal() || executeI->normal() 
        || memoryI->normal() || writeBackI->normal())
        return true;
    
    return (fetchI->prediction()!=-1);
}
