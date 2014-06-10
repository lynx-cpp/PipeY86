#include <iostream> 
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include "tools.h"
#include "check.h"
#include "build.h"

/*
 * 
 * You can run program with:
 * unix > ./s2o [INPUT_FILE] [OUTPUT_FILE]
 * 
 */

std :: map<std :: string, int> dict;
std :: map<std :: string, int> check;

int main(int argc, char **argv) {
	
	if (argc < 3){
		std :: cerr << "Need More Parameters" << std :: endl;
		return -1;
	}
	//Need More Parameters
	
	std :: ifstream fin(argv[1]);
	std :: vector<std :: string> InstructionRegister;
	InstructionRegister.erase(InstructionRegister.begin(),InstructionRegister.end());
	while (!fin.eof()){
		read_instruction(fin,InstructionRegister);
		//std :: cout << InstructionRegister[InstructionRegister.size()-1] << std :: endl;
	}
	InstructionRegister.pop_back();
	fin.close();
	//Input
	int error_row = 0;
	int error_code = check_compile(InstructionRegister,error_row);
	if (error_code) return error_code;
	//Check
	int row = 0;
	std :: ostringstream output;
	build(output,InstructionRegister,row);
	std :: ofstream fout(argv[2]);
	fout << output.str();
	fout.close();
	return 0;
}
