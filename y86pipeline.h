#ifndef Y86PIPELINE_H
#define Y86PIPELINE_H
#include <cstdio>
#include <vector>
#include <fstream>

class Instruction;
//typedef Instruction* InstrctionPtr;
//typedef std::vector<Instruction> Program;
typedef std::vector<Instruction> Program;
typedef Program::iterator InstructionPtr;


class Y86Pipeline
{
private:
    Instruction* nop;
    InstructionPtr fetchI,decodeI,executeI,memoryI,writeBackI;
    int m_register[10];
    
    char* m_memory;
    int startAddr,endAddr;
    
    bool ZeroFlag,OverflowFlag,SignFlag;
    
public:
    Y86Pipeline(const std::string& filename);
    bool running();
    void run();
    void execute(InstructionPtr nextPrediction);
    int readRegister(int num) { return m_register[num]; }
    void writeRegister(int num,int value) { m_register[num] = value; }
    Program::iterator findInstructionFromAddr(int addr);
    
    void setConditionCode(int a,int b,int val);
    bool jle() { return (SignFlag ^ OverflowFlag) | ZeroFlag; }
    bool jl() { return SignFlag ^ OverflowFlag; }
    bool e() { return ZeroFlag; }
    bool jne() { return ~ZeroFlag; }
    bool jge() { return ~(SignFlag ^ OverflowFlag); }
    bool jg() { return ~(SignFlag ^ OverflowFlag) & (~ZeroFlag); }
    //int nextInstruction(int current,int step = 1) {  }
};

#endif // Y86PIPELINE_H
