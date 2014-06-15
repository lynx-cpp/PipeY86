#include "instructionprivate.h"
#include "y86pipeline.h"

InstructionOP::InstructionOP(const std::string& instructionCode, int address):InstructionPrivate(address)
{
    icode = 6; ifun = hex2num(instructionCode[1]);
    //std::cerr << instructionCode << std::endl;
    type = other; opString = "other";
    if (ifun==0){
        type = addl;
        opString = "addl";
    }
    if (ifun==1){
        type = subl;
        opString = "subl";
    }
    if (ifun==2){
        type = andl;
        opString = "andl";
    }
    if (ifun==3){
        type = xorl;
        opString = "xorl";
    }
}

InstructionOP::~InstructionOP()
{

}

void InstructionOP::fetchStage()
{
    InstructionPrivate::fetchStage();
    if (m_instructionCode.size()<4){
        stat = INS;
        std::cerr << "invalid instruction."  << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    srcA = rA; srcB = rB;
    dstE = rB;
}

bool InstructionOP::decodeStage()
{
    InstructionPrivate::decodeStage();
    //valA = m_pipeline->readRegister(rA);
    //valB = m_pipeline->readRegister(rB);
    currentOperation = "valA <- R[rA]; valB <- R[rB];";
    return (readReg(rA,valA) && readReg(rB,valB));
}

void InstructionOP::executeStage()
{
    InstructionPrivate::executeStage();
    switch (type){
        case (addl):
            valE = valB + valA;
            break;
        case (subl):
            valE = valB - valA;
            break;
        case (andl):
            valE = valB & valA;
            break;
        case (xorl):
            valE = valB ^ valA;
            break;
        default:
            ;
    }
    m_pipeline->setConditionCode(valB,valA,valE);
    writeForwardReg(rB,valE,true);
    currentOperation = "valE <- valA " + opString + " valB;";
}

void InstructionOP::memoryStage()
{
    InstructionPrivate::memoryStage();
}

void InstructionOP::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    //m_pipeline->writeRegister(rB,valE);
    writeRealReg(rB,valE);
    currentOperation = "R[rB] <- valE;";
}

