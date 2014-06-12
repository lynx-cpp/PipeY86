#include <string>
#include <map>
#include <vector>

class AssemblerY86
{
private:
	std :: map<std :: string, int> dict;
	std :: map<std :: string, int> check;
	std :: vector<std :: string> InstructionRegister;
public:	
	AssemblerY86();
	void readFile(std :: string file_name);
	bool checkBool();
	char checkChar(int &row);
	void buildFile(std :: string file_name);
	bool compile(std :: string input_file, std::string output_file);
};
