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
#include <map>

class compiler {
public:
    compiler(std::istream*, std::ostream*); //SIMPLE code input, SML output
    virtual ~compiler();
//private: //TODO reapply private label
    std::vector<std::vector<std::string>> parse(std::istream*);
    std::vector<std::vector<std::string>> \
                               make_sml(std::vector<std::vector<std::string>>*);
    std::tuple<std::string,int> let(std::vector<std::string>*);
    static std::vector<std::string> tokenize(std::string, std::string);
    static std::vector<std::string> to_postfix(std::vector<std::string>*);
    
    std::map<std::string,int> addresses; //Map of SIMPLE code addresses to SML addresses
    //Note: addresses are stored in the form a42 for address 42
    std::vector<std::string> vars;
    //Note: constants are stored in the form c42 for number 42
};

#endif /* COMPILER_H */

