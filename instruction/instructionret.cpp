#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionRet :: InstructionRet(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
}

InstructionRet :: ~InstructionRet()
{
}

void InstructionRet :: fetchStage()
{
	InstructionPrivate :: fetchStage();
}

bool InstructionRet :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation = "valA <- R[%esp]; valB <- R[%esp];";
	return (readReg(4,valA) && readReg(4,valB));
}

void InstructionRet :: executeStage()
{
	InstructionPrivate :: executeStage();
	currentOperation = "valE <- valB + 4;";
	valE = valB + 4;
}

void InstructionRet :: memoryStage()
{
	InstructionPrivate :: memoryStage();
	currentOperation = "valM <- M_4[valA]";
	valM = m_pipeline->read32BitMemory(valA);
	writeForwardReg(4,valE,true);
}

void InstructionRet :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
	writeRealReg(4,valE);
	currentOperation = "R[%esp] <- valE;";
	valP = findInstructionFromAddr(valM);
}
