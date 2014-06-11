#include "instructionprivate.h"
#include "y86pipeline.h"

int findInstructionFromAddr(int address)
{
    int len = prog.size();
    for (int i=0;i<len;i++)
        if (prog[i].addr()==address)
            return i;
        return -1;
}

InstructionPrivate::InstructionPrivate(int address):
rA(0),rB(0),valC(0),valA(0),valB(0),valE(0),valM(0),srcA(NO_REG),srcB(NO_REG),dstE(NO_REG),dstM(NO_REG)
{
    m_address = address;
    stat = BUB;
    valP = -1;
}

bool InstructionPrivate::readReg(int num, int& dest)
{
    return m_pipeline->readForwarding(num,dest);
}

void InstructionPrivate::writeForwardReg(int num, int value, bool flag)
{
    m_pipeline->writeForwarding(num,value,flag);
}

void InstructionPrivate::writeRealReg(int num, int value)
{
    m_pipeline->writeForwarding(num,value,true);
    m_pipeline->writeRegister(num,value);
}

void InstructionPrivate::fetchStage()
{
    if (m_address==-1){
        valP = -1;
        return ;
    }
    valP = findInstructionFromAddr(m_address); valP ++; 
    currentOperation = "NOP;";
}


bool InstructionPrivate::decodeStage()
{
    currentOperation = "NOP;";
    return true;
}

void InstructionPrivate::executeStage()
{
    currentOperation = "NOP;";
}

void InstructionPrivate::memoryStage()
{
    currentOperation = "NOP;";
}

void InstructionPrivate::writeBackStage()
{
    currentOperation = "NOP;";
}


