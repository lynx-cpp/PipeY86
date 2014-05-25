#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "y86pipeline.h"
#include "instruction.h"
#include "utility.h"
#define PIPELINE

static int* mem_bak;

void Y86Pipeline::execute(InstructionPtr nextPrediction)
{
#ifndef PIPELINE
    fetchI->fetchStage();
    fetchI->decodeStage();
    fetchI->memoryStage();
    fetchI->executeStage();
    fetchI->writeBackStage();
    fetchI = nextPrediction;
    return ;
#else
    
    writeBackI->writeBackStage();
    memoryI->memoryStage();
    executeI->executeStage();
    decodeI->decodeStage();
    fetchI->fetchStage();
    
    if (executeI->isOk())
    if (executeI->prediction()!=decodeI){
        decodeI->setBubble();
        fetchI->setBubble();
        nextPrediction = executeI->prediction();
    }
    
    if (memoryI->isOk())
    if (memoryI->prediction()!=executeI){
        fetchI->setBubble();
        decodeI->setBubble();
        executeI->setBubble();
        nextPrediction = memoryI->prediction();
    }
    //prediction may change.
    
    writeBackI = memoryI;
    memoryI = executeI;
    executeI = decodeI;
    decodeI = fetchI;
    fetchI = nextPrediction;
    
    if (fetchI==NULL){
        prog.push_back(Instruction());
        fetchI = (&prog[prog.size() - 1]);
    }
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
        
        Instruction* cur = new Instruction(s2,curAddr);
        prog.push_back(*cur);
        
        for (int i=0;i+1<s2.size();i+=2){
            m_memory[curAddr - startAddr] = byte2int(s2[i],s2[i + 1]);
            curAddr ++;
        }
    }
    
    InstructionPtr it;
    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); writeBackI = it;
    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); memoryI = it;
    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); executeI = it;
    prog.push_back(Instruction()); it = (&prog[prog.size()-1]); decodeI= it;
    fetchI = (&prog[0]);
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
    for (InstructionPtr i=prog.begin();i!=prog.end();i++){
        i->setPipeline(this);
        i->printCode();
    }*/
    int len = prog.size();
    for (int i=0;i<len;i++){
        prog[i].setPipeline(this);
        prog[i].printCode();
    }
    std::cerr<< "starting Y86Pipeline.." << std::endl;
    do{
        InstructionPtr next = fetchI->prediction();
        if (next==NULL){
            prog.push_back(Instruction());
            next = (&prog[prog.size() - 1]);
        }
        std::cerr << next->instructionP << std::endl;
        execute(next);
    } while (running());
    for (int i=0;i<8;i++){
        if (m_register[i]!=0)
            std::cout << i << " " << m_register[i] << std::endl;
    }
}

bool Y86Pipeline::running()
{
    return (fetchI->isOk() || decodeI->isOk() || executeI->isOk() 
    || memoryI->isOk() || writeBackI->isOk());
}
