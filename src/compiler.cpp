/* 
 * File:   compiler.cpp
 * Author: hdamron1594
 * 
 * Created on October 24, 2016, 10:08 AM
 */

#include <fstream>
#include <vector>
#include "compiler.h"

using namespace std;

compiler::compiler(istream *input, ostream *output) {
    
}

compiler::~compiler() {
}

/**
 * Tokenizes one string into a vector of strings
 * @param str String to be tokenized
 * @param delimiter String describing location of split between tokens
 * @return Returns vector containing string tokens
 */
vector<string> compiler::tokenize(string str, string delimiter) {
    vector<string> tokens;
    int start = 0;
    int end = 0;
    while (end != string::npos) {
        end = str.find(delimiter, start);
        string sub = str.substr(start, end - start);
        if (sub.size() > 0) {
            tokens.push_back(sub);
        }
        start = end + delimiter.size();
    }
    return tokens;
}