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

//#include "y86pipeline.h"
#include "utility.h"
//typedef std::vector<Instruction> Program;
enum status {
    AOK,HLT,ADR,INS,BUB
} ;
int findInstructionFromAddr(int address);


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
    int valP;
    
 
public:
    friend class Instruction;
    InstructionPrivate(int address);
    //InstructionPrivate() {}
    //InstructionPrivate(const InstructionPrivate& ip) {  }
    void setPipeline(Y86Pipeline* pipeline) { m_pipeline = pipeline; }
    int addr() { return m_address; }
    int prediction() const { return valP; }
    
    virtual ~InstructionPrivate() {}
    
    virtual void fetchStage();
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
    friend class Y86Pipeline;
    Instruction(const std::string& instructionCode,int address);
    Instruction() 
    { 
        m_instructionCode = "00";
        m_address = -1;
        constructPrivate();
    }
    Instruction(const Instruction& ip) ;
    ~Instruction() 
    { 
        if (instructionP!=NULL) 
            delete instructionP; 
        else
            std::cerr << "InstructionPrivate Error.." << std::endl;
    }
    void setPipeline(Y86Pipeline* pipeline) { instructionP->setPipeline(pipeline); }
    int prediction() const { return instructionP->prediction(); }
    void printCode() { std::cerr << "code : " << m_instructionCode << std::endl; }
    
    void setBubble() { instructionP->stat = BUB; }
    bool isBubble() { return (instructionP->stat == BUB); }
    void setOk() { instructionP->stat = AOK; }
    bool isOk() { return (instructionP->stat==AOK); }
    bool normal() { return isOk() && (m_address!=-1); }
    
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
    int addr() { return instructionP->addr(); }
    bool operator!=(const Instruction& B);
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
