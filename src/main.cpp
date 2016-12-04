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
            "15 let x = y + x * 4 * ( 2 + y )\n"
            "20 output x\n"
            "21 end\n";
    
    compiler::compile(&simple, &cout);
//    compiler cpl;
//    vector<string> testvec = vector<string>{"24", "let", "z", "=", "y", "+", "x", "*", "4", "*", "(", "2", "+", "y", ")"};
//    tuple<string,int> test = cpl.let(&testvec);
//    cout << get<0>(test) << endl;
}