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
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>

class compiler {
public:
    static std::vector<std::string> tokenize(std::string, std::string); //tokenizes string into a vector
    static std::string replace_all(std::string, std::string, std::string); //search and replace all in string
    static std::string fmt(std::string, int size, char fill); //formats string with leading characters
    static int manual_stoi(std::string); //does stoi, but in a predictable manner
    static std::string manual_to_string(int num); //manual to_string
    static void compile(std::string, std::string); //compiles from one file into another
    static bool precheck(std::vector<std::string>); //prechecks postfix conversion
//private: 
    static std::string compile(std::istream*); //converter method makes a compiler instance and compiles
    explicit compiler();
    virtual ~compiler();
    
    std::string get_sml(std::istream*);
  
    std::vector<std::vector<std::string>> parse(std::istream*);
    std::string make_sml(std::vector<std::vector<std::string>>*);
    
    std::string input(std::vector<std::string>*);
    std::string output(std::vector<std::string>*);
    std::tuple<std::string,int> let(std::vector<std::string>*);
    std::string _goto(std::vector<std::string>*);
    std::string _if(std::vector<std::string>*);
    
    std::string second_parse(std::string);
    
    int precedence(std::string, std::string); //TODO could be const but would not compile
    std::vector<std::string> to_postfix(std::vector<std::string>); //TODO same as above about const
    
    std::vector<std::vector<std::string>> simple; //2D vector of simple code
    
    std::unordered_set<int> addresses; //Map of SIMPLE code addresses to SML addresses
    //Note: needed addresses are written in the form A42 for address 42
    std::unordered_set<std::string> vars;
    //Note: all vars are single letters in SIMPLE code, but others are allowed
    std::unordered_set<std::string> initialized_vars;
    //initialized vars must include all vars by the end
    std::unordered_set<int> constants;
    //Note: constants are written in the form A42 for number 42
    std::unordered_map<int,int> address_map;
    //Note: address map has actual addresses mapped SIMPLE to SML
    int stack_size = 0; //Number of stack memory locations necessary as S##
    int program_size = 0; //SML physical position of last command + 1
};

#endif /* COMPILER_H */
