#ifndef CHECK_H
#define CHECK_H
#include "tools.h"
#include <map>
#include <utility>
#include <string>

/*
 *
 *  Error 1 : Illegal Number
 *  Error 2 : Illegal Imm
 *  Error 3 : Illegal Register
 *  Error 4 : Illegal Label
 *  Error 5 : Reload  Label
 *  Error 6 : Unknown Symol
 *  Error 7 : Unknown Command
 *  Error 8 : Too short Line
 *
 */

static inline char check_compile(std :: vector<std :: string> InstructionRegister, int &row,std::map<std::string, int> &dict,std::map<std::string,int> &check) {
    int pos = 0;
    bool flag = false;
    for (row = 0; row < InstructionRegister.size(); ++row)
    {
        std :: vector <std :: string> line;
        line.erase(line.begin(),line.end());
        InstructionRegister[row] = remove_note(InstructionRegister[row],flag);
        InstructionRegister[row] = normalize(InstructionRegister[row]);
        split(InstructionRegister[row],line);
        if (line.size() == 0) {
            continue;
        }
        for (int iter = 0; iter < line.size(); ++iter)
        {
            do {
                if (line[iter][0] =='.')
                {
                    if (line[iter] == ".pos")
                    {
                        ++iter;
                        if (iter == line.size()) return 8;
                        if (is_not_number(line[iter])) return 1;
                        pos = string2int(line[iter]);
                    } else if (line[iter] == ".align")
                    {
                        ++iter;
                        if (iter == line.size()) return 8;
                        if (is_not_number(line[iter])) return 1;
                        int align = string2int(line[iter]);
                        pos = ((pos-1) / align + 1) * align;
                    } else if (line[iter] == ".long")
                    {
                        ++iter;
                        if (iter == line.size()) return 8;
                        if (is_not_number(line[iter])) return 1;
                        pos += 4;
                    } else return 1;
                    break;
                }
                if (line[iter][line[iter].size()-1] == ':')
                {
                    if (is_not_valid_symbol(line[iter]))
                        return 4;
                    std :: string str = line[iter].substr(0,line[iter].size()-1);
                    if (dict.find(str) != dict.end())
                        return 5;
                    dict.insert(make_pair(str,pos));
                    break;
                }
                int code = get_code(line[iter]);
                int rA,rB,valC,Dest;
                if (code != 0xe0)
                {
                    pos = pos + get_code_length(line[iter]);
                    do {
                        if (code == 0x20 || (code & 0xf0) == 0x60)
                        {
                            if (iter+2 >= line.size()) return 8;
                            if (is_not_valid_register(line[iter+1])|| is_not_valid_register(line[iter+1])) return 3;
                            iter += 2;
                            break;
                        }
                        // rrmovl opt
                        if (code == 0x30)
                        {
                            if (iter+2 >= line.size()) return 8;
                            remove_interpunction(line[iter+1]);
                            if (dict.find(line[iter+1]) == dict.end() && is_not_number(line[iter+1].substr(1,line[iter+1].size()-1)))
                                check.insert(make_pair(line[iter+1],row));
                            //return 2;
                            if (is_not_valid_register(line[iter+2])) return 3;
                            iter += 2;
                            break;
                        }
                        // irmovl
                        if (code == 0x40)
                        {
                            if (iter+2 >= line.size()) return 8;
                            if (is_not_valid_register(line[iter+1])) return 3;
                            if (!is_not_number(line[iter+2]))
                            {
                                if (iter + 3 < line.size())
                                {
                                    if (is_not_valid_register(line[iter+3])) return 3;
                                    iter += 3;
                                } else iter += 2;
                            } else
                            {
                                if (is_not_valid_register(line[iter+2])) return 3;
                                iter += 2;
                            }
                            break;
                        }
                        // rmmovl
                        if (code == 0x50)
                        {
                            if (iter+2 >= line.size()) return 8;
                            if (!is_not_number(line[iter+1]))
                            {
                                if (iter + 3 < line.size())
                                {
                                    if (is_not_valid_register(line[iter+2])) return 3;
                                    iter += 3;
                                } else iter += 2;
                            } else
                            {
                                if (is_not_valid_register(line[iter+1])) return 3;
                                iter += 2;
                            }
                            if (is_not_valid_register(line[iter])) return 3;
                            break;
                        }
                        // mrmovl
                        if ((code & 0xf0) == 0x70 || (code == 0x80))
                        {
                            if (iter +1 >= line.size()) return 8;
                            remove_interpunction(line[iter+1]);
                            if (dict.find(line[iter+1]) == dict.end())
                                check.insert(make_pair(line[iter+1],row));
                            //return 6;
                            iter += 1;
                            break;
                        }
                        //jXX call
                        if (code == 0xa0 || code == 0xb0)
                        {
                            if (iter +1 >= line.size()) return 8;
                            if (is_not_valid_register(line[iter+1])) return 3;
                            iter += 1;
                            break;
                        }
                        // pushl popl
                        break;
                    } while (true);
                    break;
                }
                if (code == 0xe0)
                {
                    return 7;
                    break;
                }
                break;

            } while(true);
        }
    }
    for (std :: map<std :: string ,int > :: iterator it = check.begin(); it != check.end() ; ++it)
    {
        std :: string label = it -> first;
        remove_interpunction(label);
        int row = it -> second;
        if (dict.find(label) == dict.end()) return 6;
    }
    // deal with check map
    return 0;
}


#endif
