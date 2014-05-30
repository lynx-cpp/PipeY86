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

class InstructionRrmovl : public InstructionPrivate
{
public:
	InstructionRrmovl(const std::string & m_instructionCode, int address);
	virtual ~InstructionRrmovl();
	virtual void fetchStage();
	virtual void decodeStage();
	virtual void executeStage();
	virtual void memoryStage();
	virtual void writeBackStage();
};

#endif // INSTRUCTION_H
