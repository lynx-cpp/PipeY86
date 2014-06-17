#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionCall :: InstructionCall(const std::string& m_instructionCode,int address):InstructionPrivate(address)
{
}

InstructionCall :: ~InstructionCall()
{
}

void InstructionCall :: fetchStage()
{
	InstructionPrivate :: fetchStage();
	if (m_instructionCode.size() < 10){
		stat = INS;
		std::cerr << "invalid instruction." << std::endl;
		//invalid instruction ...
	}
	valC = readHexSmallEndian(m_instructionCode,2,9);
	icode = 8; ifun = 0;
	srcA = 8;
	srcB = 4;
	dstE = 4;
	dstM = 8;
}

bool InstructionCall :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation =  "R[%esp] <- valB;";
	return readReg(4,valB);
}

void InstructionCall :: executeStage()
{
	InstructionPrivate :: executeStage();
	currentOperation = "valE <- valE + (-4);";
	valE = valB + (-4);
}

void InstructionCall :: memoryStage()
{
	InstructionPrivate :: memoryStage();
	currentOperation = "valE <- valP";
	m_pipeline->write32BitMemory(valE,valP);
	writeForwardReg(4,valE,true);
}

void InstructionCall :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
	currentOperation = "R[%esp] <- valE;";
	writeRealReg(4,valE);
	valP = findInstructionFromAddr(valC);
}
