#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Instruction;
typedef std::vector<Instruction> Program;
extern Program prog;

static inline int hex2num(char ch)
{
    if (ch>='0' && ch<='9') return ch - '0';
    return ch - 'a' + 10;
}

static inline int byte2int(char a,char b)
{
    return hex2num(a)*16 + hex2num(b);
}

static inline int readHexBigEndian(const std::string& str,int l,int r)
{
    int ret = 0;
    for (int i=l;i<=r;i++){
        ret *= 16;
        ret += hex2num(str[i]);
    }
    return ret;
}

static inline void readTripleStr(std::fstream& stream,std::string& s1,std::string& s2,std::string& s3)
{
    std::string line;
    std::getline(stream,line);
    std::stringstream ss;
    ss.clear(); ss.str(line);
    ss >> s1 >> s2 >> s3;
}

static inline int readHexSmallEndian(const std::string& str,int l,int r)
{
    int ret = 0;
    for (int i=r;i-1>=l;i-=2){
        ret *= 256;
        ret += byte2int(str[i - 1],str[i]);
    }
    //std::cerr << "Read hex : " << ret
    return ret;
}

//hello
#endif
