/* 
 * //TODO compiler.h overview
 * File:   src/compiler.h
 * Author: Hunter Damron and Dennis Perea
 *          (hdamron17)      (sacredden)
 *
 * Created on October 24, 2016, 10:08 AM
 */

#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>

class compiler {
public:
    compiler(std::istream*, std::ostream*); //SIMPLE code input, SML output
    virtual ~compiler();
//private: //TODO reapply private label
    std::vector<std::vector<std::string>> parse(std::istream*);
    std::vector<std::vector<std::string>> \
                               make_sml(std::vector<std::vector<std::string>>*);
    
    std::string rem(std::vector<std::string>*);
    std::string input(std::vector<std::string>*);
    std::string output(std::vector<std::string>*);
    std::tuple<std::string,int> let(std::vector<std::string>*);
    
    std::string second_parse(std::string);
    
    static std::vector<std::string> tokenize(std::string, std::string);
    static std::vector<std::string> to_postfix(std::vector<std::string>*);
    
    std::unordered_set<int> addresses; //Map of SIMPLE code addresses to SML addresses
    //Note: addresses are written in the form a42 for address 42
    std::unordered_set<std::string> vars;
    //Note: all vars are single letters in SIMPLE code, but others are allowed
    std::unordered_set<int> constants;
    //Note: constants are written in the form c42 for number 42
    int stack_size = 0;
    int program_size = 0;
};

#endif /* COMPILER_H */

