#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionMrmovl::InstructionMrmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
}

InstructionMrmovl::~InstructionMrmovl()
{
}

void InstructionMrmovl::fetchStage()
{
    InstructionPrivate::fetchStage();
    if (m_instructionCode.size()<12){
        stat = INS;
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
    icode = 5; ifun = 0;
	srcA = 8;
	srcB = rB;
	dstE = 8;
    dstM = rA;
}

bool InstructionMrmovl::decodeStage()
{
	InstructionPrivate :: decodeStage();
    currentOperation = "R[rB] <- valB;";
    return readReg(rB,valB);
}

void InstructionMrmovl::executeStage()
{
    InstructionPrivate::executeStage();
    valE = valB + valC;
    writeForwardReg(rA,valM,false);
    currentOperation = "valE <- valB + valC;";
}

void InstructionMrmovl::memoryStage()
{
    InstructionPrivate::memoryStage();
    valM = m_pipeline->read32BitMemory(valE);
    std :: cerr << "Check " << valM << std::endl;
    writeForwardReg(rA,valM,true);
    currentOperation = "valM <- M_4[valE];";
}

void InstructionMrmovl::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    writeRealReg(rA,valM);
    currentOperation = "R[rA] <- valM;";
}
