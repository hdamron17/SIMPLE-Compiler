#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "compiler.h"

using namespace std;

int main() {
//    vector<vector<string>> vec = vector<vector<string>> {
//        compiler::tokenize("10 input x", " "),
//        compiler::tokenize("12 input y", " "),
//        compiler::tokenize("13 goto 20", " "),
//        compiler::tokenize("14 rem hello steven", " "),
//        compiler::tokenize("20 output x", " "),
//        compiler::tokenize("21 end", " ")
//    };
    
//    cout << "=====\n" << test.make_sml(&vec) << "=====\n"; //TODO rem
//    cout << compiler::fmt("8",4,'0') << endl; 
//    cout << compiler::replace_all("I want to ride my bike. I want to ride my bike.", "bike", "bicycle") << endl;
    
//    string partial_sml = 
//            "10x\n"
//            "10y\n"
//            "20x\n"
//            "30y\n"
//            "40a10\n"
//            "40a4\n"
//            "20s0\n"
//            "34c881\n"
//            "32c9\n"
//            "34c0\n"
//            "21s1\n"
//            "40a8\n"
//            "40a9\n"
//            "4300\n";
//    cout << "=====\n" << test.second_parse(partial_sml) << "\n=====\n";
    
    stringstream simple;
    simple <<
            "10 input x\n"
            "12 input y\n"
            "12 goto 20\n"
            "14 rem hello steven\n"
            "20 output x\n"
            "21 end\n";
    //  cout << simple.str() << endl;
//    ofstream* file = new ofstream("output.asm",ios::out);
    compiler::compile("simple.smp", "compiled.sml");
//    file->close();
}