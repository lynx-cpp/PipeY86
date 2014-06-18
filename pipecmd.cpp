/*
 *   Copyright (C) 2014 by Yuquan Fang<lynx.cpp@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
