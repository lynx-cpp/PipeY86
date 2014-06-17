#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionPush :: InstructionPush(const std::string& instructionCode, int address):InstructionPrivate(address)
{
	icode = 0xa;
	ifun = 0;
}

InstructionPush :: ~InstructionPush()
{
}

void InstructionPush :: fetchStage()
{
	InstructionPrivate :: fetchStage();
	if (m_instructionCode.size() < 4){
		stat = INS;
		std :: cerr << "invalid instrucion." << std::endl;
		//invalid instruc ...
	}
	rA = hex2num(m_instructionCode[2]);
	srcA = rA;
    srcB = ESP;
    dstE = ESP;
}

bool InstructionPush :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation = "valA <- R[rA]; valB <- R[%esp];";
    return (readReg(rA,valA) && readReg(ESP,valB));
}

void InstructionPush :: executeStage()
{
	InstructionPrivate :: executeStage();
	valE = valB + (-4);
	currentOperation = "valE <- valB + (-4);";
    writeForwardReg(ESP,valE,true);
}

void InstructionPush :: memoryStage()
{
	InstructionPrivate :: memoryStage();
    m_pipeline->write32BitMemory(valE,valA);
}

void InstructionPush :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
    writeRealReg(ESP,valE);
	currentOperation = "R[%esp] <- valE;";
}



