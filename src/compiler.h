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

class compiler {
public:
    compiler(std::istream*, std::ostream*); //SIMPLE code input, SML output
    virtual ~compiler();
private:
    std::vector<std::vector<std::string>> parse(std::istream*);
    std::vector<std::vector<std::string>> \
                    make_sml(std::vector<std::vector<std::string>> simple_code);
    std::vector<std::string> tokenize(std::string, std::string);

};

#endif /* COMPILER_H */

