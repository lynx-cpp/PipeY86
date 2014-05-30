#include "instructionprivate.h"
#include "y86pipeline.h"

int findInstructionFromAddr(int address);
InstructionOP::InstructionOP(const std::string& instructionCode, int address):InstructionPrivate(address)
{
    type = other;
    if (ifun==0) type = addl;
    if (ifun==1) type = subl;
    if (ifun==2) type = andl;
    if (ifun==3) type = xorl;
}

InstructionOP::~InstructionOP()
{

}

void InstructionOP::decodeStage()
{
    InstructionPrivate::decodeStage();
    valA = m_pipeline->readRegister(rA);
    valB = m_pipeline->readRegister(rB);
}

void InstructionOP::executeStage()
{
    InstructionPrivate::executeStage();
    switch (type){
        case (addl):
            valE = valB + valA;
            break;
        case (subl):
            valE = valB - valA;
            break;
        case (andl):
            valE = valB & valA;
            break;
        case (xorl):
            valE = valB ^ valA;
            break;
        default:
            ;
    }
    m_pipeline->setConditionCode(valB,valA,valE);
}

void InstructionOP::fetchStage()
{
    InstructionPrivate::fetchStage();
    if (m_instructionCode.size()<4){
        stat = INS;
        std::cerr << "invalid instruction."  << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
}

void InstructionOP::memoryStage()
{
    InstructionPrivate::memoryStage();
}

void InstructionOP::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    m_pipeline->writeRegister(rB,valE);
}

InstructionPrivate::InstructionPrivate(int address):
rA(0),rB(0),valC(0),valA(0),valB(0),valE(0),valM(0)
{
    m_address = address;
    stat = BUB;
    valP = -1;
}

InstructionIrmovl::InstructionIrmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
}

void InstructionIrmovl::decodeStage()
{
    InstructionPrivate::decodeStage();
}

void InstructionIrmovl::executeStage()
{
    InstructionPrivate::executeStage();
    valE = 0 + valC;
}

void InstructionIrmovl::fetchStage()
{
    InstructionPrivate::fetchStage();
    if (m_instructionCode.size()<12){
        stat = INS;
        std::cerr << "invalid instruction."  << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
}

void InstructionIrmovl::memoryStage()
{
    InstructionPrivate::memoryStage();
}

void InstructionIrmovl::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    m_pipeline->writeRegister(rB,valE);
}

InstructionIrmovl::~InstructionIrmovl()
{
}

InstructionRrmovl :: InstructionRrmovl(const std::string& m_instructionCode, int address):InstructionPrivate(address)
{
}

InstructionRrmovl :: ~ InstructionRrmovl()
{
}

void InstructionRrmovl :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size()!=4){
        stat = INS;
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
}

void InstructionRrmovl :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    valA = m_pipeline->readRegister(rB);
}

void InstructionRrmovl :: executeStage()
{
    InstructionPrivate :: executeStage();
    valE = 0 + valA;
}

void InstructionRrmovl :: memoryStage()
{
    InstructionPrivate :: memoryStage();
}

void InstructionRrmovl :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();
    m_pipeline->writeRegister(rB,valE);
}

void InstructionPrivate::fetchStage()
{
    valP = findInstructionFromAddr(m_address); valP ++; 
}

int findInstructionFromAddr(int address)
{
    int len = prog.size();
    for (int i=0;i<len;i++)
        if (prog[i].addr()==address)
            return i;
        return -1;
}
