#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionJump :: InstructionJump(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
	switch (ifun)
	{
		case 0:
			type = jmp;
			jumpString = "jmp";
		break;
		case 1:
			type = jle;
			jumpString = "jle";
		break;
		case 2:
			type = jl;
			jumpString = "jl";
		break;
		case 3:
			type = je;
			jumpString = "je";
		break;
		case 4:
			type = jne;
			jumpString = "jne";
		break;
		case 5:
			type = jge;
			jumpString = "jge";
		break;
		case 6:
			type = jg;
			jumpString = "jg";
		break;
		default:
			type = other;
			jumpString = "other";
		break;
	}
}

InstructionJump :: ~InstructionJump()
{
}

void InstructionJump :: fetchStage()
{
	InstructionPrivate :: fetchStage();
	if (m_instructionCode.size() < 10){
		stat = INS;
		std :: cerr << "invalid instruction." << std :: endl;
		//invalid instruction ...
	}
	valC = readHexSmallEndian(m_instructionCode,2,9);
	currentOperation = "valC <- M_4[PC];";
}

bool InstructionJump :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	return true;
	//???
}

void InstructionJump :: executeStage()
{
	InstructionPrivate :: executeStage();
	switch (type)
	{
		case jmp:BCH = true; break;
		case jl:BCH = m_pipeline->jl();
		case jle:BCH = m_pipeline->jle();
		case je:BCH = m_pipeline->je();
		case jne:BCH = m_pipeline->jne();
		case jge:BCH = m_pipeline->jge();
		case jg:BCH = m_pipeline->jg();
		default: break;
	}
	currentOperation = "Bch <- Cond(CC,ifun);";
}

void InstructionJump :: memoryStage()
{
	InstructionPrivate :: memoryStage();
}

void InstructionJump :: writeBackStage()
{
	InstructionPrivate :: writeBackStage();
	if (BCH)
	{
		valP = findInstructionFromAddr(valC);
		currentOperation ="valP <- valC;";
	}
}
