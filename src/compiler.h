/* 
 * File:   compiler.h
 * Author: hdamron1594
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
    std::vector<std::string> tokenize(std::string, std::string);

};

#endif /* COMPILER_H */

