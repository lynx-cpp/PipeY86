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
    srcA = NO_REG;
    srcB = ESP;
    dstE = ESP;
    dstM = NO_REG;
}

bool InstructionCall :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation =  "R[%esp] <- valB;";
    return readReg(ESP,valB);
}

void InstructionCall :: executeStage()
{
	InstructionPrivate :: executeStage();
    currentOperation = "valE <- valB + (-4);";
	valE = valB + (-4);
    writeForwardReg(ESP,valE,true);
    returnAddr = valP;
    valP = findInstructionFromAddr(valC);
    std :: cerr << "call :  PC <- " << valP << std::endl;
    std :: cerr << "call :  PC <- " << int2Hex(valC) << std::endl;
}

void InstructionCall :: memoryStage()
{
	InstructionPrivate :: memoryStage();
	currentOperation = "valE <- valP";
    m_pipeline->write32BitMemory(valE,findAddrFromInstruction(returnAddr));
}

void InstructionCall :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
	currentOperation = "R[%esp] <- valE;";
    writeRealReg(ESP,valE);

}
