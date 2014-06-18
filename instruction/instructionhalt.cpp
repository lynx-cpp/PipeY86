#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionHalt :: InstructionHalt(const std::string& m_instructionCode, int address):InstructionPrivate(address)
{
}

InstructionHalt :: ~InstructionHalt()
{
}

void InstructionHalt::fetchStage()
{
	InstructionPrivate :: fetchStage();
}

bool InstructionHalt :: decodeStage()
{
	InstructionPrivate :: decodeStage();
}

void InstructionHalt :: executeStage()
{
	InstructionPrivate :: executeStage();
}

void InstructionHalt :: memoryStage()
{
	InstructionPrivate :: memoryStage();
}

void InstructionHalt :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
	status = HLT;
}
