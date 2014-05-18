#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "y86pipeline.h"
#include "instruction.h"
#include "utility.h"

static int* mem_bak;

void Y86Pipeline::execute(InstructionPtr nextPrediction)
{
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
    
    if (fetchI==prog.end()){
        prog.push_back(*nop);
        fetchI = prog.end() - 1;
    }
}

Y86Pipeline::Y86Pipeline(const std::string& filename)
{
    nop = new Instruction((InstructionPrivate*)NULL);
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
    prog.push_back(*nop); it = prog.end(); it--; writeBackI = it;
    prog.push_back(*nop); it = prog.end(); it--; memoryI = it;
    prog.push_back(*nop); it = prog.end(); it--; executeI = it;
    prog.push_back(*nop); it = prog.end(); it--; decodeI= it;
    fetchI = prog.begin();
    memset(m_register,0,sizeof(m_register));
    std::cerr << "initialization completed." << std::endl;
}

Program::iterator Y86Pipeline::findInstructionFromAddr(int addr)
{
    Program::iterator ret = prog.end();
    for (Program::iterator i=prog.begin();i!=prog.end();i++)
        if (i->addr()==addr)
            ret = i;
        return ret;
}

void Y86Pipeline::setConditionCode(int a, int b, int val)
{
    OverflowFlag = (a<0 == b<0) && (val<0 != a<0);
    ZeroFlag = (val == 0);
    SignFlag = (val<0);
}

void Y86Pipeline::run()
{
    for (InstructionPtr i=prog.begin();i!=prog.end();i++){
        i->setPipeline(this);
        i->printCode();
    }
    std::cerr<< "starting Y86Pipeline.." << std::endl;
    do{
        InstructionPtr next = fetchI->prediction();
        if (next==prog.end()){
            prog.push_back(*nop);
            next = prog.end();
            next --;
        }
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
