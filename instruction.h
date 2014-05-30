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
    std::string m_instructionCode;
    int m_address;
    void constructPrivate();

public:
    friend class InstructionPrivate;
    friend class Y86Pipeline;
    Instruction(const std::string& instructionCode,int address);
    Instruction();
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
    void decodeStage();
    void executeStage();
    void memoryStage();
    void writeBackStage();
    int addr();
    bool operator!=(const Instruction& B);
};

#endif