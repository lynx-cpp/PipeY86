#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionPop :: InstructionPop(const std::string& m_instructionCode, int address):InstructionPrivate(address)
{
	icode = 0xb;
	ifun = 0;
}
InstructionPop :: ~InstructionPop()
{
}

void InstructionPop :: fetchStage()
{
	InstructionPrivate :: fetchStage();
	if (m_instructionCode.size() < 4){
		stat = INS;
		std :: cerr << "invalid instrucion." << std::endl;
		//invalid instruc ...
	}
	rA = hex2num(m_instructionCode[2]);
	srcA = 4;
	srcB = 4;
	dstE = 4;
	dstM = rA;
}

bool InstructionPop :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation = "valA <- R[%esp]; valB <- R[%esp];";
	return (readReg(4,valA) && readReg(4,valB));
}

void InstructionPop :: executeStage()
{
	InstructionPrivate :: executeStage();
	valE = valB + 4;
	currentOperation = "valE <- valB + 4";
	writeForwardReg(4,valE,true);
	// the command below
	valM = m_pipeline->read32BitMemory(valA);
	writeForwardReg(rA,valM,true);
	//should be done in memoryStage()
}

void InstructionPop :: memoryStage()
{
	InstructionPrivate :: memoryStage();
	valM = m_pipeline->read32BitMemory(valA);
	currentOperation = "valM <- M_4[valA];";
}

void InstructionPop :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
	writeRealReg(4,valE);
	writeRealReg(rA,valM);
	currentOperation = "R[%esp] <- valE; R[rA] <- valM";
}
