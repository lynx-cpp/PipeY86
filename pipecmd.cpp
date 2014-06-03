#include <cstdio>
#include <iostream>
#include <fstream>

#include "y86pipeline.h"
#include "instruction.h"
#include "utility.h"

using namespace std;

Program prog;
Y86Pipeline* pipeline;

int main(int argc, char **argv) {
    if (argc<2) {
       std::cerr << "Please specify input file!" << std::endl;
       return 1;
    }
    //fstream file(argv[1],std::fstream::in);
    pipeline = new Y86Pipeline(std::string(argv[1]));
    pipeline->run();
    return 0;
}
