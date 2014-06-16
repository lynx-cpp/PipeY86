#include <iostream>
#include "instruction.h"
#include "y86pipeline.h"
#include "utility.h"
#include "instructionprivate.h"

void Instruction::constructPrivate()
{
    instructionP = NULL;
    if (m_instructionCode.length()<2){
        std::cerr << "instruction code \'"<< m_instructionCode <<"\' length too short" << std::endl;
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
        if (code==0x20){
            instructionP = new InstructionRrmovl(m_instructionCode,m_address);
            break;
        }
        if (code==0x30){
            instructionP = new InstructionIrmovl(m_instructionCode,m_address);
            break;
        }
        if (code==0x40){
			instructionP = new InstructionRmmovl(m_instructionCode,m_address);
			break;
		}
		if (code==0x50){
			instructionP = new InstructionMrmovl(m_instructionCode,m_address);
			break;
		}
        if (icode==0x7){
            instructionP = new InstructionJump(m_instructionCode,m_address);
            break;
        }
        if (code==0x80){
            instructionP = new InstructionCall(m_instructionCode,m_address);
            break;
        }
        if (code==0x90){
            instructionP = new InstructionRet(m_instructionCode,m_address);
            break;
        }
        /*
        if (code==0xa0){
            instructionP = new InstructionPush(m_instructionCode,m_address);
        }
        */
        //add new instruction constructing function here
        
        instructionP = new InstructionNop(m_address);
    } while (false);
    instructionP->m_instructionCode = m_instructionCode;
}

Instruction::Instruction(const Instruction& ip)
{
    m_instructionCode = ip.m_instructionCode;
    if (m_instructionCode=="")
        m_instructionCode = "00";
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

std::string Instruction::currentOperation()
{
    return instructionP->currentOperation;
}

#ifdef QT_VERSION
QVariantList Instruction::status() const
{
    int addr = instructionP->addr(); 
    if (addr==-1) addr = 0;
    QVariantList ret;
    ret.clear();
    ret.append(instructionP->icode);
    ret.append(instructionP->ifun);
    ret.append(instructionP->rA);
    ret.append(instructionP->rB);
    ret.append(instructionP->dstE);
    ret.append(instructionP->dstM);
    ret.append(instructionP->srcA);
    ret.append(instructionP->srcB);
    ret.append(QString::fromStdString(int2Hex(instructionP->valA,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valB,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valC,8)));
    ret.append(QString::fromStdString(int2Hex(addr,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valE,8)));
    ret.append(QString::fromStdString(int2Hex(instructionP->valM,8)));
    return ret;
}
#endif //QT_VERSION
