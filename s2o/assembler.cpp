#include "assembler.h"
#include "build.h"
#include "check.h"

AssemblerY86::AssemblerY86()
{
    dict.erase(dict.begin(),dict.end());
    check.erase(check.begin(),check.end());
    InstructionRegister.erase(InstructionRegister.begin(),InstructionRegister.end());
}

void AssemblerY86::readFile(std::string file_name)
{
    std::ifstream fin(file_name.c_str());
    while (!fin.eof()) {
        read_instruction(fin,InstructionRegister);
    }
    InstructionRegister.pop_back();
    fin.close();
}

char AssemblerY86::checkChar(int &row)
{
    int error_row = 0;
    int error_code = check_compile(InstructionRegister,error_row,dict,check);
    std::cerr << error_code << std::endl;
    return error_code;
}

bool AssemblerY86::checkBool()
{
    int error_row=0;
    return checkChar(error_row);
}


void AssemblerY86::buildFile(std::string file_name)
{
    std::ostringstream output;
    build(output,InstructionRegister,dict,check);

    std::ofstream fout(file_name.c_str());
    fout << output.str();
    fout.close();
}

bool AssemblerY86::compile(std::string input_file, std::string output_file)
{
    dict.erase(dict.begin(),dict.end());
    check.erase(check.begin(),check.end());
    InstructionRegister.erase(InstructionRegister.begin(),InstructionRegister.end());
    readFile(input_file);
    bool result = checkBool();
    if (result) return false;
    buildFile(output_file);
    return true;
}
