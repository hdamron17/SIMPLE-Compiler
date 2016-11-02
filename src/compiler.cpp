/* 
 * //TODO compiler.cpp overview
 * File:   src/compiler.cpp
 * Author: Hunter Damron and Brennan Cain
 *          (hdamron17)       (brenn10)
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

static const string CONSTANTS = "abcdefghijklmnopqrstuvwxyz1234567890";
static const string ALPHA = "abcdefghijklmnopqrstuvwxyz";
static const string NUMERIC = "1234567890";
//TODO figure out constants to prevent redundency

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
//TODO note: returns string not 2D vector of strings
string compiler::make_sml(vector<vector<string>> *simple_code) {
    //TODO process SIMPLE code vector and generate code for each term
    //TODO Note: make sure to add linenum of each command to addresses map
    stringstream sml_stream;
    for(vector<string> line : (*simple_code) ) {
        if(line.size() >= 2) {
            try {
                int linenum = stoi(line[0]);
                //TODO check that linenum is within 0 to 100
                cout << "[" << linenum << "] -> [" << program_size << "]" << endl; //TODO remove
                address_map.insert({linenum, program_size});
            } catch(invalid_argument& e) {
                //TODO handle error of invalid linenum
            } //TODO possibly also handle out_of_range for int max/min value
            string command = line[1];
            if(command == "rem") {
                //TODO put an easter egg here maybe
            } else if(command == "input") {
                sml_stream << input(&line);
                program_size++;
            } else if(command == "output") {
                sml_stream << output(&line);
                program_size++;
            } else if(command == "goto") {
                sml_stream << _goto(&line);
                program_size++;
            } else if(command == "if") {
              	
            } else if(command == "let") {
          	    tuple<string,int> cmd = let(&line);
                sml_stream << get<0>(cmd);
                program_size += get<1>(cmd);
            } else if(command == "end") {
              	//TODO possibly test if there are unnecessary terms after end
                sml_stream << "4300\n";
                program_size++;
            } else {
              	//TODO throw error for invalid command
            }
        } else {
            //TODO throw error about not enough arguments
        }
    }
    return sml_stream.str();
    //TODO second check to replace pseudoSML
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

string compiler::_goto(vector<string> *cmd) {
  	if(cmd->size() == 3) {
      	int linenum;
      	try {
            linenum = stoi(cmd->at(2));
            //TODO check that linenum is within 0 to 100
            addresses.insert(linenum);
        } catch(invalid_argument& e) {
            //TODO handle error of invalid linenum
          	linenum = -1; //TODO this will screw up further but we'll deal with that later
        } //TODO possibly also handle out_of_range for int max/min value
      	stringstream sml;
        sml << "40a" << linenum << endl; //puts temporary value starting with 'a'
      	string ret = sml.str();
        return ret;
    } else {
      	//TODO throw error on invalid number of args
    }
}

/**
 * Writes SML for let command, adding variables to instance list
 * @param cmd Tokenized SIMPLE let command (<linenum> let <var> = <term>)
 * @return Returns tuple containing (full SML string, number of SML lines)
 *
 * Algorithm:
 *	 - start after equals sign and convert to postfix
 *   if( token is numerical ) {
 *   	- load token to accumulator
 *      if( next token is numerical ) {
 *      	- store token to top of stack (labeled in temporary SML as "s1" for stack number 1 - starting at 0)
 *     		- increment stack pointer (not an actual address but a number of stack space necessary)
 *  	else if( next token is operator ) {
 * 			- do nothing yet
 * 	 if( token is operator ) {
 *  	- apply operator to top of stack (i.e. x y * sees "*" and multiplies by top of stack (representing x))
 * 		- decrement stack size pointer (to show that value is no longer needed and can be overwritten)
 *  	if( next token does not exists (i.e. is end of command) ) {
 * 			- store result to variable before equals sign
 * 		else if( next token exists ) {
 * 			- store result to top of stack and increment stack size pointer
 * 	 - possible sources of error:
 * 		- number of ops does not decrement stack pointer to zero (some values never used)
 * 		- last token is a number
 */
tuple<string,int> compiler::let(vector<string> *cmd) {
    if(cmd->size() >= 5 && cmd->at(3) == "=") {
        vector<string> infix(cmd->begin()+3, cmd->end());
//        vector<string> postfix = to_postfix(&infix);
        
        string var = cmd->at(2);
        if(ALPHA.find(var) != string::npos) {
            vars.insert(var);
        } else {
            //TODO throw error for invalid variable
        }
        stringstream sml;
     	for(auto token : /*postfix*/ infix) {
        	
        }
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
 * Gives true if op1 is of higher precedence
*/
bool compiler::higherRank(string op1, string op2) 
{
  
}

/**
 * Converts infix to postfix for easier processing
 * @param infix Tokenized string of math in infix notation
 * @return Returns the same math in postfix notation
 */
vector<string> compiler::to_postfix(vector<string> *infix) 
{
	//Queue<string> post();
  	
}

/**
 * Tokenizes one string into a vector of strings
 * @param str String to be tokenized
 * @param delimiter String describing location of split between tokens
 * @return Returns vector containing string tokens
 * @author Hunter Damron
 */
vector<string> compiler::tokenize(string str, string delimiter) {
  	//TODO check error handling for empty string
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