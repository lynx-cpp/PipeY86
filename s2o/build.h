#ifndef BUILD_H
#define BUILD_H
#include <map>
#include "tools.h"

static inline void build(std :: ostringstream &output, std :: vector<std :: string> InstructionRegister,std::map<std::string, int> dict,std::map<std::string,int> check)
{
	output.str("");
	int pos = 0;
	bool flag = false;
	for (int row = 0; row < InstructionRegister.size(); ++row)
	{
		std :: vector <std :: string> line;
		line.erase(line.begin(),line.end());
		std :: string val = InstructionRegister[row];
		InstructionRegister[row] = remove_note(InstructionRegister[row],flag);
		InstructionRegister[row] = normalize(InstructionRegister[row]);
		split(InstructionRegister[row],line);
		if (line.size() == 0) {
			format_output(output,"",val);
			continue;
		}
		for (int iter = 0; iter < line.size(); ++iter)
		{
			do {
				if (line[iter][0] == '.')
				{
					if (line[iter] == ".pos")
					{
						++iter;
						pos = string2int(line[iter]);
						format_output(output,format(pos,3,true) + ": ",val);
					} else if (line[iter] == ".align")
					{
						++iter;
						int align = string2int(line[iter]);
						pos = ((pos - 1) / align + 1) * align;
						format_output(output,format(pos,3,true) + ": ",val);
					} else if (line[iter] == ".long")
					{
						format_output(output,format(pos,3,true) + ": " + int2hex_small(string2int(line[++iter]),8),val);
						pos += 4;
					}
					break;
				}
				if (line[iter][line[iter].size()-1] == ':')
				{
					if (iter + 1 == line.size())
						format_output(output,format(pos,3,true)+": ",val);
					break;
				}
				int code = get_code(line[iter]);
				int rA,rB,valC,Dest;
				std :: string str = "";
				if (code != 0xe0)
				{
					int _pos = pos + get_code_length(line[iter]);
					do {
						if (code == 0x00 || code == 0x10 || code == 0x90)
						{
							str = format(pos,3,true)+": "+int2hex_small(code,2);
							format_output(output,str,val);
							break;
						}
						//nop halt ret
						if (code == 0x20 || (code & 0xf0) == 0x60)
						{
							rA = get_register(line[iter+1]);
							rB = get_register(line[iter+2]);
							str = format(pos,3,true) + ": "+int2hex_small(code,2)+get_halfbyte(rA,1)+get_halfbyte(rB,1);
							format_output(output,str,val);
							iter += 2;
							break;
						}
						//rrmovl opt
						if (code == 0x30)
						{
							rA = 8;
							rB = get_register(line[iter+2]);
							remove_interpunction(line[iter+1]);
							if (dict.find(line[iter+1]) != dict.end())
								valC = dict[line[iter+1]];
							else
								valC = get_imm(line[iter+1]);
							str = format(pos,3,true)+": "+int2hex_small(code,2)+get_halfbyte(rA,1)+get_halfbyte(rB,1)+int2hex_small(valC,8);
							format_output(output,str,val);
							iter += 2;
							break;
						}
						//irmovl
						if (code == 0x40)
						{
							rA = get_register(line[iter+1]);
							if (!is_not_number(line[iter+2]))
							{
								if (iter + 3 < line.size())
								{
									valC = get_imm("$"+line[iter+2]);
									rB = get_register(line[iter+3]);
									iter += 3;
								} else
								{
									valC = get_imm("$"+line[iter+2]);
									rB = 8;
									iter += 2;
								}
							} else 
							{
								valC = 0;
								rB = get_register(line[iter+2]);
								iter += 2;
							}
							str = format(pos,3,true)+": "+int2hex_small(code,2)+get_halfbyte(rA,1)+get_halfbyte(rB,1)+int2hex_small(valC,8);
							format_output(output,str,val);
							break;
						}
						//rmmovl
						if (code == 0x50)
						{
							if (!is_not_number(line[iter+1]))
							{
								if (iter + 3 < line.size())
								{
									valC = get_imm("$"+line[iter+1]);
									rB = get_register(line[iter+2]);
									iter += 3;
								} else
								{
									valC = get_imm("$"+line[iter+1]);
									rB = 8;
									iter += 2;
								}
							} else
							{
								valC = 0;
								rB = get_register(line[iter+1]);
								iter += 2;
							}
							rA = get_register(line[iter]);
							str = format(pos,3,true)+": "+int2hex_small(code,2)+get_halfbyte(rA,1)+get_halfbyte(rB,1)+int2hex_small(valC,8);
							format_output(output,str,val);
							break;
						}
						//mrmovl
						if ((code & 0xf0) == 0x70 || code == 0x80)
						{
							remove_interpunction(line[iter+1]);
							Dest = dict[line[iter+1]];
							str = format(pos,3,true)+": "+int2hex_small(code,2)+int2hex_small(Dest,8);
							format_output(output,str,val);
							iter += 1;
							break;
						}
						//jXX call
						if (code == 0xa0 || code == 0xb0)
						{
							rA = get_register(line[iter+1]);
							rB = 8;
							str = format(pos,3,true)+": "+int2hex_small(code,2)+get_halfbyte(rA,1)+get_halfbyte(rB,1);
							format_output(output,str,val);
							iter += 1;
							break;
						}
						// pushl popl
						break;
					} while (true);
					pos = _pos;
					break;
				}
				
				break;
			} while (true);
		}
	}
}

#endif
