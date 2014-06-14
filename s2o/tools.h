#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>

static inline void read_instruction(std :: istream & is,std :: vector<std :: string> & InstructionRegister){
	std :: string instruction;
	std :: getline (is,instruction);
	InstructionRegister.push_back(instruction);
}

template <typename T>

static inline char get_halfbyte(T x,int k){
	x = (x>>((k-1)<<2)) & 0xf;
	return (x < 10? x+'0' : x+'a'-10);
}

static inline std :: string int2hex_big(int x, int k = 2){
	char *bit = new char[k];
	std :: string result = "";
	for (int i = 0; i<k; ++i)
		bit[k-i-1] = get_halfbyte(x,i+1);
	for (int i = 0; i<k; i++)
		result.append(bit,i,1);
	delete []bit;
	return result;
}

static inline std :: string int2hex_small(int x,int k = 2){
	char *bit = new char[k+1];
	std :: string result = "";
	for (int i = 0; i<k; ++i)
		bit[k-i-1] = get_halfbyte(x,i+1);
	for (int i = 0; i<k; i+=2)
		result.append(bit,k-i-2,2);
	delete []bit;
	return result;
}

static inline std :: string :: size_type interpunction_find(std :: string word){
	std :: string :: size_type it1 = word.find_first_of(',');
	std :: string :: size_type it2 = word.find_first_of('(');
	std :: string :: size_type it3 = word.find_first_of(')');
	if (it1 < it2 && it1 < it3) return it1;
	if (it2 < it1 && it2 < it3) return it2;
	return it3;
}

static inline void remove_interpunction(std :: string &word){
	std :: string :: size_type it = interpunction_find(word);
	while (it < word.size()){
		word = word.substr(0,it) + word.substr(it+1,word.size()-it-1);
		it = interpunction_find(word);
	}
}

static inline std :: string normalize(std :: string str){
	int l = str.size();
	for (int i = 0; i < l; ++i)
	{
		if ('A'<=str[i] && str[i]<='Z')
			str[i] = str[i] -'A'+'a';
		if (str[i] == '\t')
			str[i] = ' ';
	}
	// Lower Letter
	std :: string :: size_type it0 = str.find_first_of('#');
	if (it0 >= str.size()) it0 = str.size();
	str = str.substr(0,it0);
	//Remove Note
	
	std :: string :: size_type it1 = str.find_first_of(',');
	if (it1 >= str.size()) it1 = str.size();
	while (it1 < str.size()){
		while ((it1 != 0) && (str[it1-1] == ' ')){
			str = str.substr(0,it1-1) + str.substr(it1,str.size()-it1);
			--it1;
		}
		if ((it1+1 != str.size()) && (str[it1+1] != ' '))
			str = str.substr(0,it1+1) + " " + str.substr(it1+1,str.size()-it1-1);
		it1 = str.find_first_of(',',it1+1);
		if (it1 >= str.size()) it1 = str.size();
	}
	//format to : XXX,_
	
	std :: string :: size_type it2 = str.find_first_of('(');
	if (it2 >= str.size()) it2 = str.size();
	while (it2 != str.size()){
		if ((it2 != 0) && (str[it2-1] != ' ')){
			str = str.substr(0,it2) + " " + str.substr(it2,str.size()-it2);
			++ it2;
		}
		while ((it2+1 < str.size()) && (str[it2+1] == ' ')){
			str= str.substr(0,it2+1) + str.substr(it2+2,str.size()-it2-2);
		}
		it2 = str.find_first_of('(',it2+1);
		if (it2 >= str.size()) it2 = str.size();
	}
	//format to : _(YYY
	
	std :: string :: size_type it3 = str.find_first_of(')');
	if (it3 >= str.size()) it3 = str.size();
	while (it3 != str.size()){
		while ((it3 != 0) && (str[it3-1] == ' ')){
			str = str.substr(0,it3-1) + str.substr(it3,str.size()-it3);
			--it3;
		}
		if ((it3+1 != str.size()) && (str[it3+1] != ' ') && (str[it3+1] != ','))
			str = str.substr(0,it3+1) + " " + str.substr(it3+1,str.size()-it3-1);
		it3 = str.find_first_of(')',it3+1);
		if (it3 >= str.size()) it3 = str.size();
	}
	//format to : ZZZ)_
	
	std :: string :: size_type it4 = str.find_first_of('.');
	if (it4 >= str.size()) it4 = str.size();
	while (it4 != str.size()){
		if ((it4 != 0) && (str[it4-1] != ' ')){
			str = str.substr(0,it4) + " " + str.substr(it4,str.size()-it4);
			-- it4;
		}
		while ((it4+1 != str.size()) && (str[it4+1] == ' ')){
			str = str.substr(0,it4+1) + str.substr(it4+2,str.size()-it4-2);;
		}
		it4 = str.find_first_of('.',it4+1);
		if (it4 >= str.size()) it4 = str.size();
	}
	//format to : .XXX

	std :: string :: size_type it5 = str.find_first_of(':');
	if (it5 >= str.size()) it5 = str.size();
	while (it5 !=str.size())
	{
		while ((it5 !=0) && (str[it5-1] == ' ')){
			str = str.substr(0,it5-1) + str.substr(it5,str.size()-it5);
			--it5;
		}
		if (it5+1 != str.size() && (str[it5+1] != ' ') && str[it5+1] != ',')
			str =str.substr(0,it5+1)+ " " + str.substr(it5+1,str.size()-it5-1);
		it5 = str.find_first_of(':',it5+1);
		if (it5 >= str.size()) it5 = str.size();
	}
	
	return str;
}

static inline int get_code(std :: string word){
	//remove_interpunction(word);
	if (word == "nop") return 0x00;
	if (word == "halt") return 0x10;
	if (word == "rrmovl") return 0x20;
	if (word == "irmovl") return 0x30;
	if (word == "rmmovl") return 0x40;
	if (word == "mrmovl") return 0x50;
	if (word == "call") return 0x80;
	if (word == "ret") return 0x90;
	if (word == "pushl") return 0xa0;
	if (word == "popl") return 0xb0;
	
	if (word == "addl") return 0x60;
	if (word == "subl") return 0x61;
	if (word == "andl") return 0x62;
	if (word == "xorl") return 0x63;
	
	if (word == "jmp") return 0x70;
	if (word == "jle") return 0x71;
	if (word == "jl") return 0x72;
	if (word == "je") return 0x73;
	if (word == "jne") return 0x74;
	if (word == "jge") return 0x75;
	if (word == "jg") return 0x76;
	
	if (word[0] == '.') return 0xc0; //Label
	if (word[0] == '#') return 0xd0;
	return 0xe0;
}

static inline int get_code_length(std :: string word) {
	if (word == "nop") return 1;
	if (word == "halt") return 1;
	if (word == "rrmovl") return 2;
	if (word == "irmovl") return 6;
	if (word == "rmmovl") return 6;
	if (word == "mrmovl") return 6;
	if (word == "call") return 5;
	if (word == "ret") return 1;
	if (word == "pushl") return 2;
	if (word == "popl") return 2;
	
	if (word == "addl") return 2;
	if (word == "subl") return 2;
	if (word == "andl") return 2;
	if (word == "xorl") return 2;
	
	if (word == "jmp") return 5;
	if (word == "jle") return 5;
	if (word == "jl") return 5;
	if (word == "je") return 5;
	if (word == "jne") return 5;
	if (word == "jge") return 5;
	if (word == "jg") return 5;
	
	if (word[0] == '.') return 0; //Label
	if (word[0] == '#') return 0;
	return 0;
}

static inline int get_register(std :: string word){
	remove_interpunction(word);
	if (word == "%eax") return 0;
	if (word == "%ecx") return 1;
	if (word == "%edx") return 2;
	if (word == "%ebx") return 3;
	if (word == "%esp") return 4;
	if (word == "%ebp") return 5;
	if (word == "%esi") return 6;
	if (word == "%edi") return 7;
	return 8;
}

static inline bool is_not_number(std :: string s){
	remove_interpunction(s);
	if (s.size() > 2 && s[1] == 'x')
	{
		for (int i = 2; i < s.size(); ++i)
			if (!isxdigit(s[i])) return true;
	} else
	{
		int i = 0;
		if (s.size() && s[0] == '-' || s[0] == '+') i++;
		for (; i < s.size(); ++i)
			if (!isdigit(s[i])) return true;
	}
	return false;
}

static inline int string2int(std :: string str){
	std :: istringstream stin(str);
	int result;
	if (str.size() > 1 && str[1]=='x') stin >> std::hex >> result;
	else stin >> result;
	return result;
}

static inline int get_imm(std :: string word){
	remove_interpunction(word);
	word = word.substr(1,word.size()-1);
	return string2int(word);
}

static inline void split(std :: string inputstring, std :: vector<std :: string> &line)
{
	std :: istringstream input(inputstring);
	std :: string word;
	line.erase(line.begin(),line.end());
	while (input >> word){
		line.push_back(word);
	}
}

static inline bool is_not_valid_symbol(std :: string s)
{
	return false;
	if (s == "init") return true;
	if (s == "main") return true;
	if (s == "array") return true;
	if (s == "end") return true;
	if (s == "stack") return true;
	return false;
}

static inline bool is_not_valid_register(std :: string s)
{
	return 8 == get_register(s);
}

static inline std :: string format(int val, int len, bool pre)
{
	std :: string ret = "";
	if (pre) ret = "0x";
	ret += int2hex_big(val,len);
	return ret;
}

static inline void format_output(std :: ostringstream &output, std :: string str, std :: string val, int w = 24)
{
	output << std :: setw(w) << std :: setiosflags(std :: ios :: left) << std :: setfill(' ') << str;
	output << "|" << val << std :: endl;
}

static inline std::string remove_note(std::string str,bool &flag)
{
	std::string result = "";
	int l = str.size();
	int i = 0;
	while (i < l)
	{
		if (i+1 < l && str[i] == '/' && str[i+1] =='*' && !flag)
		{
			flag = true;
			i+=2;
			continue;
		}
		if (i+1 < l && str[i] == '*' && str[i+1] =='/' && flag)
		{
			flag = false;
			i+=2;
			continue;
		}
		if (!flag) result.push_back(str[i]);
		++i;
	}
	return result;
}

#endif