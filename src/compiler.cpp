/* 
 * //TODO compiler.cpp overview
 * File:   src/compiler.cpp
 * Author: Hunter Damron and Brennan Cain
 *          (hdamron17)       (brenn10)
 * Algorithm from: http://csis.pace.edu/~wolf/CS122/infix-postfix.htm
 * Created on October 24, 2016, 10:08 AM
 */

#include <fstream>
#include <vector>
#include <stack>
#include <sstream>
#include <exception>
#include "compiler.h"

//TODO remove unnecessary imports
#include <iostream>

using namespace std;

static const string ALPHA = "abcdefghijklmnopqrstuvwxyz";

void compiler::compile(string infile, string outfile) {
    ifstream* in = new ifstream(infile, ios::in);
    ofstream* out = new ofstream(outfile,ios::out);
    compile(in, out);
    out->close();
    delete in;
    delete out;
}

// note that ostream and istream will not be closed within this. 
void compiler::compile(istream *in, ostream *out) {
    compiler cpl;
    string sml = cpl.get_sml(in);
    (*out) << sml;
}

string compiler::get_sml(istream *in)
{
    vector<vector<string>> input = parse(in);
    string pseudoSML = make_sml(&input);
    string realSML = second_parse(pseudoSML);
    return realSML;
}

/**
 * Prevents external instantiation
 */
compiler::compiler() {}

/**
 * Default deconstructor
 */
compiler::~compiler() {}

/**
 * Parses data from istream into 2D vector for processing
 * @param input Input istream with SIMPLE code
 * @return Returns 2D array with each statement on a row and each word a term
 */
vector<vector<string>> compiler::parse(istream *input) {
    vector<vector<string>> parsed;
    while(!input->eof()){
        string in;
        getline((*input),in);
        vector<string> line = tokenize(in," ");
        parsed.push_back(line);
    }
    return parsed;
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
              	if(linenum>99 or linenum<0) {
                	cerr<<"Program too large on line " << linenum<<endl;
                  	exit(EXIT_FAILURE);
                }
                address_map.insert({linenum, program_size});
            } catch(invalid_argument& e) {
                cerr<<"Invalid line number " <<line[0]<<endl;
                exit(EXIT_FAILURE);
            } catch(exception e)
            {
              	cerr<<"Line number too large at "<< line[0] << endl;
                exit(EXIT_FAILURE);
            }
            string command = line[1];
            if(command == "rem") {
                sml_stream << "; ";
                for(int i =2; i<line.size(); i++)
                {
              		sml_stream << line[i] << " ";
                }
              	sml_stream << endl;
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
              	sml_stream << _if(&line);
                // No need to change program size because Brennan did in the function
            } else if(command == "let") {
          	tuple<string,int> cmd = let(&line);
                sml_stream << get<0>(cmd);
                program_size += get<1>(cmd);
            } else if(command == "end") {
                sml_stream << "4300\n";
                program_size++;
            } else {
              	cerr << "Command \"" << line[1] << "\" invalid."<<endl;
              	exit(EXIT_FAILURE);
            }
        } else {
            try {
                int linenum = stoi(line.at(0));
              	if(linenum>99 or linenum<0) {
                    cerr<<"Program too large on line.\nToo few Arguments. Line: " << linenum<<endl;
                    exit(EXIT_FAILURE);
                }
                address_map.insert({linenum, program_size});
            } catch(out_of_range& e) {
                //TODO error checking ... Brennan
            } catch(invalid_argument& e) {
                cerr<<"Invalid line number after " << line[0] << ". Too few arguements."<<endl;
                exit(EXIT_FAILURE);
            } catch(exception e) {
              	cerr<<"Line number too large after " << line[0] << ". Too few arguements."<<endl;
                exit(EXIT_FAILURE);
            }
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
            cerr << "Invalid id name \"" << var << " on line " << cmd->at(0) << endl;
          	exit(EXIT_FAILURE);
        }
        stringstream sml;
        sml << "10" << var << endl;
        return sml.str();
    } else {
        cerr<<"Invalid argument to input on line " << cmd->at(0)<<endl;
      	exit(EXIT_FAILURE);
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
            cerr<<"Invalid variable \"" << var << "\" on line " << cmd->at(0)<<endl;
      		exit(EXIT_FAILURE);
        }
        stringstream sml;
        sml << "11" << var << endl;
        return sml.str();
    } else {
        cerr<<"Invalid command format on line " << cmd->at(0)<<endl;
        exit(EXIT_FAILURE);
    }
}

string compiler::_if(vector<string> *cmd) 
{
    if(cmd->size()==7)
    {
      	//Get id1
        string id1 = cmd->at(2);
        if(ALPHA.find(id1) != string::npos) {
            vars.insert(id1);
        } else {
            cerr<<"Invalid variable \"" << id1 << "\" on line " << cmd->at(0)<<endl;
      		exit(EXIT_FAILURE);
        }
      	//get id2
      	string id2 = cmd->at(4);
        if(ALPHA.find(id2) != string::npos) {
            vars.insert(id2);
        } else {
            cerr<<"Invalid variable \"" << id2 << "\" on line " << cmd->at(0)<<endl;
      		exit(EXIT_FAILURE);
        }
      	//get the address
      	int address;
      	try {
            address = stoi(cmd->at(6));
            if(address>99 or address<0)
            {
                cerr<<"Goto address out of bounds " << cmd->at(0)<<endl;
                exit(EXIT_FAILURE);
            }
        } catch(invalid_argument& e) {
            cerr<<"Invalid goto address on line " << cmd->at(0)<<endl;
      		exit(EXIT_FAILURE);
        }
        addresses.insert(address);
        
        stringstream sml;
        string relop = cmd->at(3);
        if(relop == "==") {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "42a" << address << endl;
            program_size += 3;
        } else if(relop == "!=") {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "42" << program_size+5 << endl;
            sml << "40a" << address<<endl;
            program_size += 4;
        } else if(relop == ">=") {
            sml << "20" << id2 << endl;
            sml << "31" << id1 << endl;
            sml << "41a" << address << endl;
            sml << "42a" << address << endl;
            program_size += 4;
        } else if(relop == "<=") {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "41a" << address << endl;
            sml << "42a" << address << endl;
            program_size += 4;
        } else if(relop == ">") {
            sml << "20" << id2 << endl;
            sml << "31" << id1 << endl;
            sml << "41a" << address<< endl;
            program_size += 3;
        } else if(relop == "<") {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "41a" << address<< endl;
            program_size += 3;
        } else {
            //TODO KILL SELF
        }
        return sml.str();
    } else {
        //TODO error on invalid number of argments
    }
}

string compiler::_goto(vector<string> *cmd) {
  	if(cmd->size() == 3) {
      	int linenum;
      	try {
            linenum = stoi(cmd->at(2));
            if(linenum>99 or linenum<0)
            {
          		cerr<<"Line pointer out of bounds on line " << cmd->at(0)<<endl;
      			exit(EXIT_FAILURE);
            }
            addresses.insert(linenum);
        } catch(invalid_argument& e) {
            cerr<<"Invalid line number on line " << cmd->at(0)<<endl;
      		exit(EXIT_FAILURE);
        }
      	stringstream sml;
        sml << "40a" << linenum << endl; //puts temporary value starting with 'a'
      	string ret = sml.str();
        return ret;
    } else {
      	cerr<<"Invalid number of arguments on line " << cmd->at(0)<<endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Tests if value is a number or a valid variable name
 * @param value String to be evaluated as numerical
 * @return Returns true if the value is a valid numerical symbol
 */
bool numerical(string value) {
    if(ALPHA.find(value) != string::npos)
        return true;
    try {
        stoi(value);
        return true;
    } catch(exception& e) {
        return false;
    }
}

/**
 * Writes SML for let command, adding variables to instance list
 * @param cmd Tokenized SIMPLE let command (<linenum> let <var> = <term>)
 * @return Returns tuple containing (full SML string, number of SML lines)
 *
 * Algorithm:
 *      - start after equals sign and convert to postfix
 *  if( token is numerical ):
 *      - load token to accumulator
 *      if( next token is numerical ):
 *          - store token to top of stack (labeled in temporary SML as "s1" for stack number 1 - starting at 0)
 *          - increment stack pointer (not an actual address but a number of stack space necessary)
 *  	else if( next token is operator ):
 *          - do nothing yet
 *  if( token is operator ):
 *  	- apply operator to top of stack (i.e. x y * sees "*" and multiplies by top of stack (representing x))
 * 	- decrement stack size pointer (to show that value is no longer needed and can be overwritten)
 *  	if( next token does not exists (i.e. is end of command) ):
 *          - store result to variable before equals sign
 * 	else if( next token exists ) {
 *          - store result to top of stack and increment stack size pointer
 *  - possible sources of error:
 *      - number of ops does not decrement stack pointer to zero (some values never used)
 *      - last token is a number
 *      - negative constants
 */
tuple<string,int> compiler::let(vector<string> *cmd) {
    if(cmd->size() >= 5 && cmd->at(3) == "=") {
        vector<string> infix(cmd->begin()+4, cmd->end());
        vector<string> postfix = to_postfix(infix);
        
        string var = cmd->at(2);
        if(ALPHA.find(var) != string::npos) {
            vars.insert(var);
        } else {
            cerr<<"Invalid ID on line " << cmd->at(0)<<endl;
        	exit(EXIT_FAILURE);
        }
        stringstream sml;
	string operators="+-/*";
        int stack_ptr = 0, cmd_size = 0;
     	for(auto token = postfix.begin(); token < postfix.end(); token++) {
            if(operators.find(*token) != string::npos && token->size() == 1) {
                sml << (30 + operators.find(*token)) << "s" << stack_ptr-1 <<endl;
                stack_ptr--;
                cmd_size++;
                if(token+1 == postfix.end()) {//checks if at end
                    sml << "21" << var << endl;
                    cmd_size++;
                } else if( !(operators.find(*(token+1)) != string::npos && token->size() == 1) ){ //checks if next is an op
                    sml << "21s" << stack_ptr << endl;
                    stack_ptr++;
                    cmd_size++;
                }
            } else {
                if(ALPHA.find(*token) != string::npos && token->size() == 1) {//checks if the next is an id
                    vars.insert(*token);
                    sml << "20" << *token << endl;
                    cmd_size++;
                } else {
                    try {
                        int num = stoi(*token);
                        sml << "20c" << num << endl;
                        constants.insert(num);
                        cmd_size++;
                    } catch(invalid_argument& e) {
                        //TODO catch invalid argument
                    } catch(out_of_range& e) {
                        //TODO catch out of range
                    }
                }
                
                if(token+1 > postfix.end()) {
                    //TODO you have a problem if it ends in a number
                } else if(operators.find(*(token+1)) != string::npos) {
                    //TODO nothing because next is an operator
                } else if(numerical(*(token+1))) {//checks if next is numerical
                    sml << "21s" << stack_ptr << endl;
                    stack_ptr++;
                    cmd_size++;
                }
            }
            if(stack_ptr > stack_size)
                stack_size = stack_ptr;
        }
        return make_tuple(sml.str(), cmd_size);
    } else {
      	cerr << "Invalid command format on line "<<cmd->at(0)<<endl;
      	exit(EXIT_FAILURE);
    }
}

/**
 * Replaces temporary variable names with physical addresses for final SML code
 *      -Also checks that SML code fits within 100 op limit
 * @param partial_sml SML code containing variable names
 * @return Returns complete SML code with all variable names replaced
 */
/**
 * Pseudocode algorithm
 * -check that 'program_size' + 'stack_size' + size('vars') + size('constants') <= 100
 * -for each value in 'addresses':
 * 		-look up sml equivalent in 'address_map'
 * 		-search and replace "a(simple address)" to "(sml address)"
 * 			-i.e. SIMPLE 40 maps to SML 8 so replace "a40" with "08"
 * -for each value in 'constants':
 * 		-search and replace "c(constant)" to "(program_size)"
 * 		-append the constant value with four digits using leading zeros to new line in the SML output
 * 		-incrememnt the 'program_size'
 * -for loop incrementing from 0 to 'stack_size' (index value is called 'i' here):
 * 		-search and replace "s(i)" to "(program_size)"
 * 		-append "0000" to new line in SML output
 * 		-incrememnt the 'program_size'
 * -for each value in 'vars':
 * 		-search and replace "(var)" to "(program_size)"
 * 		-incrememnt 'program_size'
 * -return modified string with true SML output
 */
string compiler::second_parse(string partial_sml) {
    if(program_size + stack_size + vars.size() + constants.size() > 100) {
        //TODO throw errors here
    }
    //Replace addresses
    for(auto iter = addresses.begin(); iter != addresses.end(); iter++) {
      	try {
            int replace = address_map.at(*iter);
            string newstr = fmt(to_string(replace), 2, '0');
            partial_sml = replace_all(partial_sml, "a" + to_string(*iter), newstr);
        } catch (exception e) {
          	cerr << "Out of range exception thrown due to invalid reference to position in array (add="<<&iter<<"). DUMP FOLLOWS: "<<endl;
          	for(auto i : addresses)
            {
            	cerr << i<<endl;
            }
          	exit(EXIT_FAILURE);
        }
    }
    
    //Replace constants
    for(auto iter = constants.begin(); iter != constants.end(); iter++) {
        string newstr = fmt(to_string(program_size), 2, '0');
        partial_sml = replace_all(partial_sml, "c" + to_string(*iter), newstr)
                + fmt(to_string(*iter), 4, '0') + "\n";
        program_size++;
    }
    
    //Replace stack variables
    for(int i = 0; i < stack_size; i++) {
        string newstr = fmt(to_string(program_size), 2, '0');
        partial_sml = replace_all(partial_sml, "s" + to_string(i), newstr) + "0000\n"; //TODO remove added zeros
        program_size++; 
    }
    
    for(string var : vars) {
        string newstr = fmt(to_string(program_size), 2, '0');
        partial_sml = replace_all(partial_sml, var, newstr) + "0000\n"; // TODO remove added zeros
        program_size++;
    }
  
    return partial_sml;
}

string compiler::replace_all(string str, string oldstr, string newstr) {
    int oldsize = oldstr.size();
    int strsize = str.size();
    stringstream ret;
    int prev = 0;
    for(int i = 0; i + oldsize <= strsize; i++) {
        if(oldstr == str.substr(i, oldsize)) {
            ret << str.substr(prev, i-prev) << newstr;
            i += oldsize;
            prev = i;
        }
    }
    ret << str.substr(prev, strsize-1);
    return ret.str();
}

string compiler::fmt(string original, int size, char fill) {
    int origsize = original.size();
    if(origsize < size) {
        for(int i = 0; i < size - origsize; i++) {
            original = fill + original;
        }
    }
    return original;
}
      
/**
 * Gives which op is of higher precedence
 * @param op1 first operation
 * @param op2 second operation
 * @return int 1: first is higher, 0: both are equal, -1: second is higher
 * @author Brennan Cain
*/
int compiler::precedence(string op1, string op2)
{
	string t1="^";
	string t2="*/";
	string t3="+-";
	if( (t1.find(op1)!=string::npos and t1.find(op2)!=string::npos) or \
		(t2.find(op1)!=string::npos and t2.find(op2)!=string::npos) or \
		(t3.find(op1)!=string::npos and t3.find(op2)!=string::npos))
	{
		return 0;
	}
	else if(t1.find(op1)!=string::npos)
	{
		return 1;
	}
	else if(t1.find(op2)!=string::npos)
	{
		return -1;
	}
	else if(t2.find(op1)!=string::npos)
	{
		return 1;
	}
	else if(t2.find(op2)!=string::npos)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/**
 * Converts a vector of infix to a queue of postfix
 * @param infix vector<string> golds the infix string broken on spaces
 * @return queue<string> postfixed statement
 * @author Brennan Cain
*/
vector<string> compiler::to_postfix(vector<string> infix) 
{
	string operators="-*/+()";
	vector<string> post;
	stack<string> ops;
	for(auto i : infix)
	{
		if(operators.find(i)==string::npos)//If a constant
		{
			post.push_back(i);
		}
		else if(ops.empty())//If the op stack is empty, add the op
		{
			ops.push(i);
		}
		else if(ops.top()=="(" or i=="(")//If the new op or top op are left paren, add new op
		{
			ops.push(i);
		}
		else if(i==")")//If a right paren, pop ops until left paren
		{
			while(ops.top()!="(")
			{
				post.push_back(ops.top());
				ops.pop();
			}
			ops.pop();
		}
		else if(precedence(i,ops.top())==1)//if new op has higher precedence, push it
		{
			ops.push(i);
		}
		else if(precedence(i,ops.top())==0)//if the new op has equal precedence, pop and push new
		{
			post.push_back(ops.top());
			ops.pop();
			ops.push(i);
		}
		else if(precedence(i,ops.top())==-1) //If new is lower, solve until new is equal or higher
		{
			while(precedence(i,ops.top())==-1)
			{
				post.push_back(ops.top());
				ops.pop();
				if(ops.empty())//Prevents segmentation faults
				{
					break;
				}
			}
			ops.push(i);
		}
	}
	while(!ops.empty())//Flush the rest of the ops
	{
		post.push_back(ops.top());
		ops.pop();
	}
	return post;
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

