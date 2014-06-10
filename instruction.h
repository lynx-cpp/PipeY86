#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <cstring>
#include <string>
#include <vector>
#include <iostream>


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
    friend class Y86Pipeline;
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
};

#endif
