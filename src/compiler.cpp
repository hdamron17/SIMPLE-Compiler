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
#include <sstream>
#include "compiler.h"

//TODO remove unnecessary imports
#include <iostream>

using namespace std;

static const string ALPHA = "abcdefghijklmnopqrstuvwxyz";
static const string NUMERIC = "1234567890";

/**
 * Compiler with input stream and output stream
 * @param input Input stream with SIMPLE code
 * @param output Output stream to which to write SML code
 */
compiler::compiler(istream *input, ostream *output) {
    //TODO constructor and function call organization
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
 */
vector<vector<string>> compiler::parse(istream *input) {
    //TODO parse method form two dimensional array with each new line in a row
    //  and each word in a line as an element in the row (using tokenize() )
}

/**
 * Generates 2D SML code vector from 2D SIMPLE code vector
 * @param simple_code 2D vector containing SIMPLE code (produced by parse() )
 * @return Returns 2D vector containing SML code in same format as simple_code
 */
vector<vector<string>> compiler::make_sml(vector<vector<string>> *simple_code) {
    //TODO process SIMPLE code vector and generate code for each term
    //TODO Note: make sure to add linenum of each command to addresses map
    for(vector<string> line : (*simple_code) ) {
        //TODO parse each command individually
        if(line.size() >= 3) {
            try {
                int linenum = stoi(line[0]);
                //TODO check that linenum is within 0 to 100
                address_map.insert({linenum, program_size});
            } catch(invalid_argument& e) {
                //TODO handle error of invalid linenum
            } //TODO possibly also handle out_of_range for int max/min value
            string command = line[1];
            if(command == "rem") {
                
            } else if(command == "input") {
                
            } else if(command == "output") {
                
            } else if(command == "goto") {
                
            } 
        } else {
            //TODO throw error about not enough arguments
        }
    }
}

/**
 * Writes SML for input command, putting filler for variable address
 * @param cmd Tokenized SIMPLE input command (<linenum> input <var>)
 * @return Returs full SML string (always a single line for input command)
 */
string compiler::input(vector<string> *cmd) {
    if(cmd->size() == 3) {
        string var = cmd->at(2);
        if(ALPHA.find(var) != string::npos) {
            vars.insert(var);
        } else {
            //TODO throw error for invalid variable
        }
        stringstream sml;
        sml << "10" << var << endl;
        return sml.str();
    } else {
        //TODO throw exception for invalid command format
    }
}

/**
 * Writes SML for outpt command, putting filler for variable address
 * @param cmd Tokenized SIMPLE output command (<linenum> output <var>)
 * @return Returs full SML string (always a single line for output command)
 */
string compiler::output(vector<string> *cmd) {
    if(cmd->size() == 3) {
        string var = cmd->at(2);
        if(ALPHA.find(var) != string::npos) {
            vars.insert(var);
        } else {
            //TODO throw error for invalid variable
        }
        stringstream sml;
        sml << "11" << var << endl;
        return sml.str();
    } else {
        //TODO throw exception for invalid command format
    }
}

/**
 * Writes SML for let command, adding variables to instance list
 * @param cmd Tokenized SIMPLE let command (<linenum> let <var> = <term>)
 * @return Returns tuple containing (full SML string, number of SML lines)
 */
tuple<string,int> compiler::let(vector<string> *cmd) {
    if(cmd->size() >= 5 && cmd->at(3) == "=") {
        vector<string> infix(cmd->begin()+3, cmd->end());
        vector<string> postfix = to_postfix(&infix);
        
        string var = cmd->at(2);
        if(ALPHA.find(var) != string::npos) {
            vars.insert(var);
        } else {
            //TODO throw error for invalid variable
        }
        stringstream sml;
        //TODO put stuff into accumulator
    } else {
        //TODO throw exception for invalid command format
    }
}

/**
 * Replaces temporary variable names with physical addresses for final SML code
 *      -Also checks that SML code fits within 100 op limit
 * @param partial_sml SML code containing variable names
 * @return Returns complete SML code with all variable names replaced
 */
string compiler::second_parse(std::string partial_sml) {
    //TODO replace all variables and addresses with actual values, also check
    //      for memory overflow
}

/**
 * Converts infix to postfix for easier processing
 * @param infix Tokenized string of math in infix notation
 * @return Returns the same math in postfix notation
 */
vector<string> compiler::to_postfix(vector<string> *infix) {
    //TODO convert infix notation to postfix
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