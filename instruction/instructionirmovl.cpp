#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionIrmovl::InstructionIrmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
}

InstructionIrmovl::~InstructionIrmovl()
{
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
    icode = 3; ifun = 0;
    dstE = rB;
}

bool InstructionIrmovl::decodeStage()
{
    InstructionPrivate::decodeStage();
}

void InstructionIrmovl::executeStage()
{
    InstructionPrivate::executeStage();
    valE = 0 + valC;
    writeForwardReg(rB,valE,true);
    currentOperation = "valE <- 0 + valC;";
}


void InstructionIrmovl::memoryStage()
{
    InstructionPrivate::memoryStage();
}

void InstructionIrmovl::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    //m_pipeline->writeRegister(rB,valE);
    writeRealReg(rB,valE);
    currentOperation = "R[rB] <- valE;";
}

