#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionRmmovl :: InstructionRmmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
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
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
    icode = 4; ifun = 0;
    srcA = rA;
	srcB = rB;
}

bool InstructionRmmovl :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation = "R[rA] <- valA; R[rB] <- valB;";
	return (readReg(rA,valA) && readReg(rB,valB));
}

void InstructionRmmovl :: executeStage()
{
	InstructionPrivate :: executeStage();
	currentOperation = "valE <- valB + valC;";
	valE = valB + valC;
}

void InstructionRmmovl :: memoryStage()
{
	InstructionPrivate :: memoryStage();
	currentOperation = "M_4[valE] <- valA;";
	std :: cerr << valA << std::endl;
	m_pipeline->write32BitMemory(valE,valA);
}

void InstructionRmmovl :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
}
