/* 
 * //TODO compiler.cpp overview
 * File:   src/compiler.cpp
 * Author: Hunter Damron and Dennis Perea
 *          (hdamron17)      (sacredden)
 * 
 * Created on October 24, 2016, 10:08 AM
 */

#include <fstream>
#include <vector>
#include "compiler.h"

using namespace std;

/**
 * Compiler with input stream and output stream
 * @param input Input stream with SIMPLE code
 * @param output Output stream to which to write SML code
 * @author Hunter Damron
 */
compiler::compiler(istream *input, ostream *output) {
    
}

/**
 * Default deconstructor
 */
compiler::~compiler() {
}

/**
 * Parses data from istream into 2D vector for processing
 * @param input Input istream with SIMPLE code
 * @return Returns 2D array with each statement on a row and each word a term
 * @author Hunter Damron
 */
vector<vector<string>> compiler::parse(istream *input) {
    //TODO parse method form two dimensional array with each new line in a row
    //  and each word in a line as an element in the row (using tokenize() )
}

/**
 * Generates 2D SML code vector from 2D SIMPLE code vector
 * @param simple_code 2D vector containing SIMPLE code (produced by parse() )
 * @return Returns 2D vector containing SML code in same format as simple_code
 * @author //TODO
 */
vector<vector<string>> compiler::make_sml(vector<vector<string>> simple_code) {
    //TODO process SIMPLE code vector and generate code for each term
}

/**
 * Tokenizes one string into a vector of strings
 * @param str String to be tokenized
 * @param delimiter String describing location of split between tokens
 * @return Returns vector containing string tokens
 * @author Hunter Damron
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