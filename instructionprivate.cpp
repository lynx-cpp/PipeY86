#include "instructionprivate.h"
#include "y86pipeline.h"

int findInstructionFromAddr(int address);

/*---------------------*/
/*     Operation       */
/*---------------------*/

InstructionOP::InstructionOP(const std::string& instructionCode, int address):InstructionPrivate(address)
{
    srcA = rA; srcB = rB;
    dstE = rB;
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


/*---------------------*/
/*    Operation End    */
/*---------------------*/



/*---------------------*/
/*       Private       */
/*---------------------*/

InstructionPrivate::InstructionPrivate(int address):
rA(0),rB(0),valC(0),valA(0),valB(0),valE(0),valM(0),srcA(NO_REG),srcB(NO_REG),dstE(NO_REG),dstM(NO_REG)
{
    m_address = address;
    stat = BUB;
    valP = -1;
}

bool InstructionPrivate::readReg(int num, int& dest)
{
    return m_pipeline->readForwarding(num,dest);
}

void InstructionPrivate::writeForwardReg(int num, int value, bool flag)
{
    m_pipeline->writeForwarding(num,value,flag);
}

void InstructionPrivate::writeRealReg(int num, int value)
{
    m_pipeline->writeForwarding(num,value,true);
    m_pipeline->writeRegister(num,value);
}

void InstructionPrivate::fetchStage()
{
    if (m_address==-1){
        valP = -1;
        return ;
    }
    valP = findInstructionFromAddr(m_address); valP ++; 
    currentOperation = "NOP;";
}


bool InstructionPrivate::decodeStage()
{
    currentOperation = "NOP;";
    return true;
}

void InstructionPrivate::executeStage()
{
    currentOperation = "NOP;";
}

void InstructionPrivate::memoryStage()
{
    currentOperation = "NOP;";
}

void InstructionPrivate::writeBackStage()
{
    currentOperation = "NOP;";
}


/*---------------------*/
/*     Private  End    */
/*---------------------*/



/*---------------------*/
/*       Irmovl        */
/*---------------------*/



InstructionIrmovl::InstructionIrmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
    dstE = rB;
}

InstructionIrmovl::~InstructionIrmovl()
{
}

void InstructionIrmovl::fetchStage()
{
    InstructionPrivate::fetchStage();
    if (m_instructionCode.size()<12){
        stat = INS;
        std::cerr << "invalid instruction."  << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
}

bool InstructionIrmovl::decodeStage()
{
    InstructionPrivate::decodeStage();
}

void InstructionIrmovl::executeStage()
{
    InstructionPrivate::executeStage();
    valE = 0 + valC;
    writeForwardReg(rB,valE,true);
    currentOperation = "valE <- 0 + valC;";
}


void InstructionIrmovl::memoryStage()
{
    InstructionPrivate::memoryStage();
}

void InstructionIrmovl::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    //m_pipeline->writeRegister(rB,valE);
    writeRealReg(rB,valE);
    currentOperation = "R[rB] <- valE;";
}



/*---------------------*/
/*     Irmovl End      */
/*---------------------*/



/*---------------------*/
/*       Rrmovl        */
/*---------------------*/


InstructionRrmovl :: InstructionRrmovl(const std::string& m_instructionCode, int address):InstructionPrivate(address)
{
    srcA = rA;
    dstE = rB;
}

InstructionRrmovl :: ~ InstructionRrmovl()
{
}

void InstructionRrmovl :: fetchStage()
{
    InstructionPrivate :: fetchStage();
    if (m_instructionCode.size()<4){
        stat = INS;
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
}

bool InstructionRrmovl :: decodeStage()
{
    InstructionPrivate :: decodeStage();
    //valA = m_pipeline->readRegister(rB);
    currentOperation = "R[rA] <- valA;";
    return readReg(rA,valA);
}

void InstructionRrmovl :: executeStage()
{
    InstructionPrivate :: executeStage();
    valE = 0 + valA;
    writeForwardReg(rB,valE,true);
    currentOperation = "R[rB] <- valE;";
}

void InstructionRrmovl :: memoryStage()
{
    InstructionPrivate :: memoryStage();
}

void InstructionRrmovl :: writeBackStage()
{
    InstructionPrivate :: writeBackStage();
    //m_pipeline->writeRegister(rB,valE);
    writeRealReg(rB,valE);
}


/*---------------------*/
/*     Rrmovl End      */
/*---------------------*/


/*---------------------*/
/*       Mrmovl        */
/*---------------------*/


InstructionMrmovl::InstructionMrmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
    srcA = rB;
    dstM = rA;
}

InstructionMrmovl::~InstructionMrmovl()
{
}

void InstructionMrmovl::fetchStage()
{
    InstructionPrivate::fetchStage();
    if (m_instructionCode.size()<12){
        stat = INS;
        std :: cerr << "invalid instruction." << std::endl;
        //invalid instruction ...
    }
    rA = hex2num(m_instructionCode[2]);
    rB = hex2num(m_instructionCode[3]);
    valC = readHexSmallEndian(m_instructionCode,4,11);
}

bool InstructionMrmovl::decodeStage()
{
	InstructionPrivate :: decodeStage();
    currentOperation = "R[rB] <- valB;";
    return readReg(rB,valB);
}

void InstructionMrmovl::executeStage()
{
    InstructionPrivate::executeStage();
    valE = valB + valC;
    writeForwardReg(rA,valM,false);
    currentOperation = "valE <- valB + valC;";
}

void InstructionMrmovl::memoryStage()
{
    InstructionPrivate::memoryStage();
    valM = m_pipeline->read32BitMemory(valE);
    writeForwardReg(rA,valM,true);
    currentOperation = "valM <- M_4[valE];";
}

void InstructionMrmovl::writeBackStage()
{
    InstructionPrivate::writeBackStage();
    writeRealReg(rA,valM);
    currentOperation = "R[rA] <- valM;";
}


/*---------------------*/
/*     Mrmovl End      */
/*---------------------*/

/*---------------------*/
/*       Rmmovl        */
/*---------------------*/

InstructionRmmovl :: InstructionRmmovl(const std::string& m_instructionCode, int address): InstructionPrivate(address)
{
    srcA = rA;
    dstM = rB;
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
}

bool InstructionRmmovl :: decodeStage()
{
	InstructionPrivate :: decodeStage();
	currentOperation = "R[rA] <- valA; R[rB] <- valB;";
	return (readReg(rA,valA) && readReg(rB,valB));
}

void InstructionRmmovl :: executeStage()
{
	InstructionRmmovl :: executeStage();
	currentOperation = "valE <- valB + valC;";
	valE = valB + valC;
}

void InstructionRmmovl :: memoryStage()
{
	InstructionRmmovl :: memoryStage();
	currentOperation = "M_4[valE] <- valA;";
	m_pipeline->write32BitMemory(valE,valA);
}

void InstructionRmmovl :: writeBackStage()
{
	InstructionRmmovl :: writeBackStage();
}

/*---------------------*/
/*     Rmmovl End      */
/*---------------------*/



int findInstructionFromAddr(int address)
{
    int len = prog.size();
    for (int i=0;i<len;i++)
        if (prog[i].addr()==address)
            return i;
        return -1;
}