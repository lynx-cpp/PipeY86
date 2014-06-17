#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include "assembler.h"
/*
 *
 * You can run program with:
 * unix > ./s2o [INPUT_FILE] [OUTPUT_FILE]
 *
 */

int main(int argc, char ** argv) {
    if (argc < 3) {
        std :: cerr << "Need more parameters" << std :: endl;
        return -1;
    }
    std::string input_file(argv[1]);
    std::string output_file(argv[2]);
    AssemblerY86 Ass;
    return Ass.compile(input_file,output_file);
}
