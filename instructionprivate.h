#ifndef INSTRUCTION_PRIVATE_H
#define INSTRUCTION_PRIVATE_H
#include "instruction.h"

class InstructionPrivate
{
protected:
    int m_address;
    //Instruction* m_father;
    Y86Pipeline* m_pipeline;
    std::string m_instructionCode,currentOperation;
    int code;
    int icode,ifun;
    status stat;
    int rA,rB,valC,valA,valB,valE,valM;
    int valP;
    
    bool readReg(int num,int& dest);
    void writeRealReg(int num,int value);
    void writeForwardReg(int num,int value,bool flag);
    
 
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
    virtual bool decodeStage();
    virtual void executeStage();
    virtual void memoryStage();
    virtual void writeBackStage();
};

class InstructionNop : public InstructionPrivate
{
public:
    InstructionNop(int address):InstructionPrivate(address) {}
    virtual ~InstructionNop() {};
};


class InstructionOP : public InstructionPrivate
{
private:
    enum OPType {
        addl,subl,andl,xorl,other
    } type;
    std::string opString;
public:
    InstructionOP(const std::string& instructionCode,int address);
    virtual ~InstructionOP();
    virtual void fetchStage();
    virtual bool decodeStage();
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
    virtual bool decodeStage();
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
    virtual bool decodeStage();
    virtual void executeStage();
    virtual void memoryStage();
    virtual void writeBackStage();
};

class InstructionMrmovl : public InstructionPrivate
{
public:
    InstructionMrmovl(const std::string& m_instructionCode,int address);
    virtual ~InstructionMrmovl();
    virtual void fetchStage();
    virtual bool decodeStage();
    virtual void executeStage();
    virtual void memoryStage();
    virtual void writeBackStage();
};

#endif