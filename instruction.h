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

#include "y86pipeline.h"
//typedef std::vector<Instruction> Program;
enum status {
    AOK,HLT,ADR,INS,BUB
} ;

class InstructionPrivate
{
protected:
    int m_address;
    //Instruction* m_father;
    Y86Pipeline* m_pipeline;
    std::string m_instructionCode;
    int code;
    int icode,ifun;
    status stat;
    int rA,rB,valC,valA,valB,valE,valM;
    Program::iterator valP;
    
 
public:
    friend class Instruction;
    InstructionPrivate(int address);
    //InstructionPrivate() {}
    //InstructionPrivate(const InstructionPrivate& ip) {  }
    void setPipeline(Y86Pipeline* pipeline) { m_pipeline = pipeline; }
    int addr() { return m_address; }
    Program::iterator prediction() { return valP; }
    
    virtual ~InstructionPrivate() {}
    
    virtual void fetchStage() { valP = m_pipeline->findInstructionFromAddr(m_address); valP ++; }
    virtual void decodeStage() {}
    virtual void executeStage() {}
    virtual void memoryStage() {}
    virtual void writeBackStage() {}
};

class InstructionNop : public InstructionPrivate
{
public:
    InstructionNop(int address):InstructionPrivate(address) {}
    virtual ~InstructionNop() {};
};

class Instruction
{
private:
    InstructionPrivate* instructionP;
    std::string m_instructionCode;
    int m_address;
    void constructPrivate();
   
public:
    friend class InstructionPrivate;
    Instruction(const std::string& instructionCode,int address);
    Instruction(InstructionPrivate* ip) 
    { 
        instructionP = ip;
        if (ip==NULL)
            instructionP = new InstructionNop(m_address);
        instructionP->m_instructionCode = "";
    }
    Instruction(const Instruction& ip) ;
    ~Instruction() { if (instructionP!=NULL) delete instructionP; }
    void setPipeline(Y86Pipeline* pipeline) { instructionP->setPipeline(pipeline); }
    Program::iterator prediction() { return instructionP->prediction(); }
    void printCode() { std::cerr << "code : " << m_instructionCode << std::endl; }
    
    void setBubble() { instructionP->stat = BUB; }
    bool isBubble() { return (instructionP->stat == BUB); }
    void setOk() { instructionP->stat = AOK; }
    bool isOk() { return (instructionP->stat==AOK); }
    
    void fetchStage() 
    { 
        if (instructionP->stat==BUB) { 
            instructionP->stat = AOK;
            instructionP->fetchStage(); 
        }
    }
    void decodeStage()  {  if (instructionP->stat==AOK) instructionP->decodeStage(); }
    void executeStage() {  if (instructionP->stat==AOK) instructionP->executeStage(); }
    void memoryStage()  {  if (instructionP->stat==AOK) instructionP->memoryStage(); }
    void writeBackStage() { if (instructionP->stat==AOK) instructionP->writeBackStage(); }
    int addr() { instructionP->addr(); }
    Program::iterator predict() { return instructionP->prediction(); }
};


class InstructionOP : public InstructionPrivate
{
private:
    enum OPType {
        addl,subl,andl,xorl,other
    } type;
public:
    InstructionOP(const std::string& instructionCode,int address);
    virtual ~InstructionOP();
    virtual void fetchStage();
    virtual void decodeStage();
    virtual void executeStage();
    virtual void memoryStage();
    virtual void writeBackStage();
};

class InstructionIrmovl : public InstructionPrivate
{
public:
    InstructionIrmovl(const std::string& m_instructionCode,int address);
    virtual ~InstructionIrmovl();
    virtual void fetchStage();
    virtual void decodeStage();
    virtual void executeStage();
    virtual void memoryStage();
    virtual void writeBackStage();
};

#endif // INSTRUCTION_H
