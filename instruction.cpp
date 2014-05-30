#include <iostream>
#include "instruction.h"
#include "y86pipeline.h"
#include "utility.h"

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

void Instruction::constructPrivate()
{   
    instructionP = NULL;
    if (m_instructionCode.length()<2){
        std::cerr << "instruction code length too short" << std::endl;
        instructionP = new InstructionNop(m_address);
        instructionP->m_instructionCode = m_instructionCode;
        return ;
    }
    int code = byte2int(m_instructionCode[0],m_instructionCode[1]);
    int icode = hex2num(m_instructionCode[0]);
    int ifun = hex2num(m_instructionCode[1]);
    do{
        if (icode==6){
            instructionP = new InstructionOP(m_instructionCode,m_address);
            break;
        }
        if (code==0x30){
            instructionP = new InstructionIrmovl(m_instructionCode,m_address);
            break;
        }
        
        //add new instruction constructing function here
        
        instructionP = new InstructionNop(m_address);
    } while (false);
    instructionP->m_instructionCode = m_instructionCode;
}

Instruction::Instruction(const Instruction& ip)
{
    m_instructionCode = ip.m_instructionCode;
    m_address = ip.m_address;
    constructPrivate();
    instructionP->setPipeline(ip.instructionP->m_pipeline);
    instructionP->stat = ip.instructionP->stat;
    instructionP->rA = ip.instructionP->rA;
    instructionP->rB = ip.instructionP->rB;
    instructionP->valA = ip.instructionP->valA;
    instructionP->valB = ip.instructionP->valB;
    instructionP->valC = ip.instructionP->valC;
    instructionP->valE = ip.instructionP->valE;
    instructionP->valM = ip.instructionP->valM;
    instructionP->valP = ip.instructionP->valP;
}

Instruction::Instruction(const std::string& instructionCode,int address)
{
    m_instructionCode = instructionCode;
    m_address = address;
    constructPrivate();
}

Instruction::Instruction()
{ 
    m_instructionCode = "00";
    m_address = -1;
    constructPrivate();
}

Instruction::~Instruction()
{ 
    if (instructionP!=NULL) 
        delete instructionP; 
    else
        std::cerr << "InstructionPrivate Error.." << std::endl;
}

void Instruction::setPipeline(Y86Pipeline* pipeline)
{
    instructionP->setPipeline(pipeline); 
}

int Instruction::prediction() const
{
    return instructionP->prediction(); 
}

void Instruction::printCode()
{
    std::cerr << "code : " << m_instructionCode << std::endl; 
}

void Instruction::setBubble()
{
    instructionP->stat = BUB; 
}

bool Instruction::isBubble()
{
    return (instructionP->stat == BUB); 
}

void Instruction::setOk()
{
    instructionP->stat = AOK;
}

bool Instruction::isOk()
{
    return (instructionP->stat==AOK);
}

bool Instruction::normal()
{
    return isOk() && (m_address!=-1); 
}

void Instruction::fetchStage()
{ 
    if (instructionP->stat==BUB) { 
        instructionP->stat = AOK;
        instructionP->fetchStage(); 
    }
}

void Instruction::decodeStage()
{
    if (instructionP->stat==AOK) instructionP->decodeStage(); 
}

void Instruction::executeStage()
{
    if (instructionP->stat==AOK) instructionP->executeStage(); 
}

void Instruction::memoryStage()
{
    if (instructionP->stat==AOK) instructionP->memoryStage(); 
}

void Instruction::writeBackStage()
{
    if (instructionP->stat==AOK) instructionP->writeBackStage(); 
}

int Instruction::addr()
{
    return instructionP->addr(); 
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

bool Instruction::operator!=(const Instruction& B)
{
    return m_address!=B.m_address;
}
