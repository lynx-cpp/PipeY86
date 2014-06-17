#ifndef INSTRUCTION_PRIVATE_H
#define INSTRUCTION_PRIVATE_H
#include "instruction.h"

int findInstructionFromAddr(int address);
int findAddrFromInstruction(int address);

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
    
    int srcA,srcB,dstE,dstM;
    
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

class InstructionRmmovl : public InstructionPrivate
{
public:
    InstructionRmmovl(const std::string& m_instructionCode,int address);
    virtual ~InstructionRmmovl();
    virtual void fetchStage();
    virtual bool decodeStage();
    virtual void executeStage();
    virtual void memoryStage();
    virtual void writeBackStage();
};

class InstructionJump : public InstructionPrivate
{
private:
	enum jumpType {
		jmp,jle,jl,je,jne,jge,jg,other
	} type;
	std::string jumpString;
	bool BCH;
public:
	InstructionJump(const std::string& m_instructionCode,int address);
	virtual ~InstructionJump();
	virtual void fetchStage();
	virtual bool decodeStage();
	virtual void executeStage();
	virtual void memoryStage();
	virtual void writeBackStage();
};

class InstructionCall :public InstructionPrivate
{
public:
	InstructionCall(const std::string& m_instructionCode,int address);
	virtual ~InstructionCall();
	virtual void fetchStage();
	virtual bool decodeStage();
	virtual void executeStage();
	virtual void memoryStage();
	virtual void writeBackStage();
};

class InstructionRet : public InstructionPrivate
{
public:
	InstructionRet(const std::string& m_instructionCode,int address);
	virtual ~InstructionRet();
	virtual void fetchStage();
	virtual bool decodeStage();
	virtual void executeStage();
	virtual void memoryStage();
	virtual void writeBackStage();
};

class InstructionPush : public InstructionPrivate
{
public:
	InstructionPush(const std::string& m_instructionCode,int address);
	virtual ~InstructionPush();
	virtual void fetchStage();
	virtual bool decodeStage();
	virtual void executeStage();
	virtual void memoryStage();
	virtual void writeBackStage();
};

class InstructionPop : public InstructionPrivate
{
public:
	InstructionPop(const std::string &m_instructionCode, int address);
	virtual ~InstructionPop();
	virtual void fetchStage();
	virtual bool decodeStage();
	virtual void executeStage();
	virtual void memoryStage();
	virtual void writeBackStage();
};

class InstructionHalt : public InstructionPrivate
{
public:
	InstructionHalt(const std::string& m_instructionCode, int address);
	virtual ~InstructionHalt();
	virtual void fetchStage();
	virtual bool decodeStage();
	virtual void executeStage();
	virtual void memoryStage();
	virtual void writeBackStage();
};

#endif
