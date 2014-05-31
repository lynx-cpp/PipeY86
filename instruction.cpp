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
<<<<<<< HEAD
	InstructionPrivate :: fetchStage();
	if (m_instructionCode.size()<4){
		stat = INS;
		std :: cerr << "invalid instruction." << std::endl;
		//invalid instruction ...
	}
	rA = hex2num(m_instructionCode[2]);
	rB = hex2num(m_instructionCode[3]);
=======
    return isOk() && (m_address!=-1); 
>>>>>>> ab0d0ad4c60095bd3c7d1711ea190f0b93043da4
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

<<<<<<< HEAD
InstructionRmmovl :: InstructionRmmovl(const std:string& m_instructionCode, int address):InstructionPrivate(address)
{
}

InstructionRmmovl :: ~InstructionRmmovl()
{
}

void InstructionRmmovl :: fetchStage()
{
	InstructionPrivate :: fetchStage();
	if (m_instructionCode.size()<12){
		stat = INS;
		std :: cerr << "invalid instruction." << std :: endl;
		//invalid instruction
	}
	rA=hex2num(m_instructionCode[2]);
	rB=hex2num(m_instructionCode[3]);
	valC = readHexSmallEndian(m_instructionCode,4,11);
}

void InstructionRmmovl :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	valA = m_pipeline->readRegister(rA);
	valB = m_pipeline->readRegister(rB);
}

void InstructionRmmovl :: executeStage()
{
	InstructionPrivate :: executeStage();
	valE = valB + valC;
}

void InstructionRmmovl :: memoryStage()
{
	InstructionPrivate:: memoryStage();
	m_pipeline ->
}

void InstructionPrivate::fetchStage()
=======
void Instruction::writeBackStage()
>>>>>>> ab0d0ad4c60095bd3c7d1711ea190f0b93043da4
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
