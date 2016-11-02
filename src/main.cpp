/* 
 * Main file for SIMPLE compiler
 * File:   src/main.cpp
 * Author: Hunter Damron and Brennan Cain
 *          (hdamron17)       (brenn10)
 * 
 * Created on October 20, 2016, 10:37 AM
 */
#include <iostream>
#include <vector>

#include "compiler.h"

void unittest() {
    //TODO unit test for testing code in intermediate stages
}

/*
 * Main function for using SIMPLE compiler
 */
int main(int argc, char** argv) {
    unittest();
    
    //TODO remove test code
    std::string teststr = 
            "14 input y\n"
            "18 input x\n"
            "20 goto 30\n"
            "25 input z\n"
            "26 rem this is a comment\n"
            "30 output x\n"
            "32 output y\n"
            "35 end\n";
    std::vector<std::string> testvec1 = compiler::tokenize(teststr, "\n");
    std::vector<std::vector<std::string>> testvec2;
    for(auto line : testvec1) {
        testvec2.push_back(compiler::tokenize(line, " "));
    }
    compiler testcplr(&std::cin, &std::cout);
    std::cout << testcplr.make_sml(&testvec2);
    
    return 0;
}
