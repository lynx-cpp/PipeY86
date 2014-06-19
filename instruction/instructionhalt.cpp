#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionHalt::InstructionHalt(const std::string& m_instructionCode,int address):InstructionPrivate(address)
{
    codeLength = 1;
}

bool InstructionHalt::decodeStage()
{
    return InstructionPrivate::decodeStage();
}

void InstructionHalt::executeStage()
{
    InstructionPrivate::executeStage();
}

void InstructionHalt::fetchStage()
{
    InstructionPrivate::fetchStage();
    stat = HLT;
    valP = -1;
    icode = 1; ifun = 0;
    std::cerr << "now begin to halt..." << std::endl;
}

void InstructionHalt::memoryStage()
{
    InstructionPrivate::memoryStage();
}

void InstructionHalt::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    std::cerr << "halt now! right now!" << std::endl;
}

InstructionHalt::~InstructionHalt()
{

}
