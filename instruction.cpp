#include <iostream>
#include "instruction.h"
#include "y86pipeline.h"
#include "utility.h"
#include "instructionprivate.h"

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
        if (code==0x20){
            instructionP = new InstructionRrmovl(m_instructionCode,m_address);
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
    m_comment = ip.m_comment;
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

Instruction::Instruction(const std::string& instructionCode, const std::string& comment, int address):m_comment(comment)
{
    m_instructionCode = instructionCode;
    m_address = address;
    constructPrivate();
}

Instruction::Instruction(int address)
{ 
    m_instructionCode = "00";
    m_address = address;
    constructPrivate();
    m_instructionCode = "";
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

bool Instruction::decodeStage()
{
    if (instructionP->stat==AOK) return instructionP->decodeStage(); 
    return true;
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

bool Instruction::operator!=(const Instruction& B)
{
    return m_address!=B.m_address;
}

bool Instruction::eq(Instruction* decodeI)
{
    if (addr()==-1)
        return true;
    return addr()==decodeI->addr();
}

std::string Instruction::instructionCode()
{
    return m_instructionCode;
}

std::string Instruction::comment()
{
    return m_comment;
}

