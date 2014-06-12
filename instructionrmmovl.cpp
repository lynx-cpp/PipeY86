#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionRmmovl :: InstructionRmmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
    srcA = rA;
    dstM = rB;
}

InstructionRmmovl :: ~InstructionRmmovl()
{

}

void InstructionRmmovl :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size()<12){
        stat = INS;
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
}

bool InstructionRmmovl :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation = "R[rA] <- valA; R[rB] <- valB;";
	return (readReg(rA,valA) && readReg(rB,valB));
}

void InstructionRmmovl :: executeStage()
{
	InstructionRmmovl :: executeStage();
	currentOperation = "valE <- valB + valC;";
	valE = valB + valC;
}

void InstructionRmmovl :: memoryStage()
{
	InstructionRmmovl :: memoryStage();
	currentOperation = "M_4[valE] <- valA;";
	m_pipeline->write32BitMemory(valE,valA);
}

void InstructionRmmovl :: writeBackStage()
{
	InstructionRmmovl :: writeBackStage();
}
