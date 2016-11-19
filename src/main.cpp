#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "compiler.h"

using namespace std;

int main() {    
    stringstream simple;
    simple <<
            "10 input x\n"
            "12 input y\n"
            "13 if x > y goto 20\n"
            "14 rem hello steven\n"
            "15 let z = y + x * 4\n"
            "20 output x\n"
            "21 end\n";
    
//    compiler::compile("simple.smp", "compiled.sml");
    compiler::compile(&simple, &cout);
}