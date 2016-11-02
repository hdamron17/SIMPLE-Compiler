/* 
 * //TODO compiler.h overview
 * File:   src/compiler.h
 * Author: Hunter Damron and Brennan Cain
 *          (hdamron17)       (brenn10)
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
    
    static std::vector<std::string> tokenize(std::string, std::string);
//private: //TODO reapply private label
    std::vector<std::vector<std::string>> parse(std::istream*);
    std::string make_sml(std::vector<std::vector<std::string>>*);
    
  //TODO figure out what we need to do for each command (function may only be necessary for let command)
    std::string input(std::vector<std::string>*);
    std::string output(std::vector<std::string>*);
    std::tuple<std::string,int> let(std::vector<std::string>*);
    std::string _goto(std::vector<std::string>*);
    
    std::string second_parse(std::string);
    
    static std::vector<std::string> to_postfix(std::vector<std::string>*);
    static bool higherRank(std::string op1, std::string op2);//gives true if op1 is of higher precedence
    
    std::unordered_set<int> addresses; //Map of SIMPLE code addresses to SML addresses
    //Note: needed addresses are written in the form a42 for address 42
    std::unordered_set<std::string> vars;
    //Note: all vars are single letters in SIMPLE code, but others are allowed
    std::unordered_set<int> constants;
    //Note: constants are written in the form c42 for number 42
    std::unordered_map<int,int> address_map;
    //Note: address map has actual addresses mapped SIMPLE to SML
    int stack_size = 0; //Number of stack memory locations necessary
    int program_size = 0; //SML physical position of last command + 1
};

#endif /* COMPILER_H */
