/* 
 * //TODO compiler.cpp overview
 * File:   src/compiler.cpp
 * Author: Hunter Damron and Brennan Cain
 *          (hdamron17)       (brenn10)
 * Created on October 24, 2016, 10:08 AM
 */

#include <fstream>
#include <vector>
#include <stack>
#include <sstream>
#include <exception>
#include <iostream>
#include "compiler.h"

using namespace std;

static const string ALPHA = "abcdefghijklmnopqrstuvwxyz";

/**
 * Takes two file names, create input/out put streams, calls hidden constructor, closes streams
 * 
 * @param infile Input file name
 * @param outfile Output file name
 * 
 * @return void
 */
void compiler::compile(string infile, string outfile) 
{
    //open streams from filenames
    ifstream* in = new ifstream(infile, ios::in);
    ofstream* out = new ofstream(outfile,ios::out);
    if(!(*in))
    {
        cerr << "Invalid input file\n";
        exit(EXIT_FAILURE);
    }
    if(!(*out))
    {
        cerr << "Invalid output file\n";
        exit(EXIT_FAILURE);
    }
    
    //call compiler
    compile(in, out);

    //close stream and delete
    out->close();
    delete in;
    delete out;
}

/**
 * Takes input stream, runs through the interpreter, outputs to out
 *
 * @param *in Input stream
 * @param *out Output stream
 *
 * @return void
 */
void compiler::compile(istream *in, ostream *out) 
{
    //create the compiler as an object
    compiler cpl;
    //compile and save to string
    string sml = cpl.get_sml(in);
    //push to out
    (*out) << sml;
}

/**
 * Takes an input stream and calls both parses on it converting to SML
 *
 * @param *in Input Stream
 *
 * @return SML code as a string
 */
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
 * Default destructor
 */
compiler::~compiler() {}

/**
 * Parses data from istream into 2D vector for processing
 *
 * @param input Input istream with SIMPLE code
 *
 * @return Returns 2D vector with each statement on a row and each word a term
 */
vector<vector<string>> compiler::parse(istream *input) 
{
    //declare return variable
    vector<vector<string>> parsed;
    //for each line
    while(!input->eof())
    {
        string in;
        
      	//Get the line
        getline((*input),in);
      	//split and save to a vector
        vector<string> line = tokenize(in," ");
      
      	//push the vector to the return variable
        parsed.push_back(line);
        
    }
    return parsed;
}

/**
 * Generates 2D SML code vector from 2D SIMPLE code vector
 *
 * @param simple_code 2D vector containing SIMPLE code (produced by parse())
 *
 * @return Returns string containing SML code in same format as simple_code
 */
string compiler::make_sml(vector<vector<string>> *simple_code) 
{
    stringstream sml_stream;
    //go through each line of the program
    for(vector<string> line : (*simple_code) ) 
    {
      	//If the line is valid
        if(line.size() >= 2) 
        {
            try //convert linenum as a string to a int
            {
                int linenum = stoi(line[0]);
              	if(linenum>99 or linenum<0) 
                {
                    cerr<<"Program too large on line " << linenum<<endl;
                    exit(EXIT_FAILURE);
                }
                address_map.insert({linenum, program_size});
            } 
            catch(invalid_argument& e) //bad linenum
            {
                cerr<<"Invalid line number " <<line[0]<<endl;
                exit(EXIT_FAILURE);
            } 
            catch(exception e)//even worse linenum
            {
              	cerr<<"Line number too large at "<< line[0] << endl;
                exit(EXIT_FAILURE);
            }
            
            //get the basic command
            string command = line[1];
            if(command == "rem") // rem
            {
                //rem ; may be extended on future versions of interpreter
//                sml_stream << "; ";
//                for(int i =2; i<line.size(); i++)
//                {
//                    sml_stream << line[i] << " ";
//                }
//              	sml_stream << endl;
            } 
          	else if(command == "input") //input
            {
                sml_stream << input(&line);
                program_size++;
            } 
          	else if(command == "print") //print
            {
                sml_stream << output(&line);
                program_size++;
            } 
          	else if(command == "goto") //goto
            {
                sml_stream << _goto(&line);
                program_size++;
            } 
          	else if(command == "if") // if
            {
              	sml_stream << _if(&line);
            } 
          	else if(command == "let") // let
            {
          	tuple<string,int> cmd = let(&line);
                sml_stream << get<0>(cmd);
                program_size += get<1>(cmd);
            } 
          	else if(command == "end") //DIE!!! (actually finish nicely)
            {
                sml_stream << "4300\n";
                program_size++;
            } 
          	else //invalid command
            {
              	cerr << "Command \"" << line[1] << "\" invalid."<<endl;
              	exit(EXIT_FAILURE);
            }
        } 
        else if(line.size() == 0)
        {
            //Do nothing on empty line
        }
      	else //line is invalid
        {
            cerr << "Invalid line " << line.at(0) << endl;
            exit(EXIT_FAILURE);
        }
    }
    return sml_stream.str();
}

/**
 * Writes SML for input command, putting filler for variable address
 *
 * @param cmd Tokenized SIMPLE input command (<linenum> input <var>)
 *
 * @return Returs full SML string (always a single line for input command)
 */
string compiler::input(vector<string> *cmd) 
{
    //Check that the line is the correct length for a input "# input id"
    if(cmd->size() == 3) 
    {
      	//get the id
        string var = cmd->at(2);
      
      	//check if id or constant
        if(ALPHA.find(var) != string::npos) 
        {
            vars.insert(var);
        } 
      	else //if not a valid id
        {
            cerr << "Invalid id \"" << var << "\" on line " << cmd->at(0) << endl;
            exit(EXIT_FAILURE);
        }
      
        stringstream sml;
      	//create the sml code
        sml << "10" << var << endl;
        return sml.str();
    } 
    else //invalid command
    {
        cerr<<"Invalid argument to input on line " << cmd->at(0)<<endl;
      	exit(EXIT_FAILURE);
    }
}

/**
 * Writes SML for outpt command, putting filler for variable address
 *
 * @param cmd Tokenized SIMPLE output command (<linenum> output <var>)
 *
 * @return Returs full SML string (always a single line for output command)
 */
string compiler::output(vector<string> *cmd) 
{
    //Check if valid command length
    if(cmd->size() == 3) 
    {
      	//get the id to output
        string var = cmd->at(2);
      
      	//check if valid id
        if(ALPHA.find(var) != string::npos) 
        {
            vars.insert(var);
        } 
      	else //if not a valid id
        {
            cerr<<"Invalid variable \"" << var << "\" on line " << cmd->at(0)<<endl;
            exit(EXIT_FAILURE);
        }
      
      	//create the sml for the output
        stringstream sml;
        sml << "11" << var << endl;
        return sml.str();
    } 
    else //BAD JUJU
    {
        cerr<<"Invalid command format on line " << cmd->at(0)<<endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Writes SML for outpt command, putting filler for variable address
 *
 * @param cmd Tokenized SIMPLE output command (<linenum> output <var>)
 *
 * @return Returs full SML string (always a single line for output command)
 */
string compiler::_if(vector<string> *cmd) 
{
    //check that the comman is in a valid format
    if(cmd->size()==7)
    {
      	//Get id1
        string id1 = cmd->at(2);
        if(ALPHA.find(id1) != string::npos) //check if a valid id
        {
            vars.insert(id1);
        } 
      	else //If not valid, die
        {
            cerr<<"Invalid variable \"" << id1 << "\" on line " << cmd->at(0)<<endl;
            exit(EXIT_FAILURE);
        }
      
      	//get id2
      	string id2 = cmd->at(4);
        if(ALPHA.find(id2) != string::npos) //check if valid id
        {
            vars.insert(id2);
        } 
      	else //if not valid, die
        {
            cerr<<"Invalid variable \"" << id2 << "\" on line " << cmd->at(0)<<endl;
            exit(EXIT_FAILURE);
        }
      
      	//get the address
      	int address;
      	try //get address if valid
        {
            address = stoi(cmd->at(6));
            if(address>99 or address<0)//address out of bounds, die
            {
                cerr<<"Goto address out of bounds " << cmd->at(0)<<endl;
                exit(EXIT_FAILURE);
            }
        } 
      	catch(invalid_argument& e) //bad address, die
        {
            cerr<<"Invalid goto address on line " << cmd->at(0)<<endl;
            exit(EXIT_FAILURE);
        }
        addresses.insert(address);
        
      	/*
         *Add ye ol' sml to the code and increment the program size
         */
        stringstream sml;
        string relop = cmd->at(3); // get the relop from the line
      
      
      	/*
         * This is a bunch of if statements for each relop
         * the SML is similar but loads the ids in different
         * order and uses different branching
         */
        if(relop == "==") 
        {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "42A" << address << endl;
            program_size += 3;
        } 
        else if(relop == "!=") 
        {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "42" << program_size+5 << endl;
            sml << "40A" << address<<endl;
            program_size += 4;
        } 
        else if(relop == ">=") 
        {
            sml << "20" << id2 << endl;
            sml << "31" << id1 << endl;
            sml << "41A" << address << endl;
            sml << "42A" << address << endl;
            program_size += 4;
        } 
        else if(relop == "<=") 
        {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "41A" << address << endl;
            sml << "42A" << address << endl;
            program_size += 4;
        } 
        else if(relop == ">") 
        {
            sml << "20" << id2 << endl;
            sml << "31" << id1 << endl;
            sml << "41A" << address<< endl;
            program_size += 3;
        } 
        else if(relop == "<") 
        {
            sml << "20" << id1 << endl;
            sml << "31" << id2 << endl;
            sml << "41A" << address<< endl;
            program_size += 3;
        } 
      	else //the relop was not specified or specified incorrectly, thus we DIE (like hunter will if he keeps doing todos)
        {
            cerr << "Relop issues on line " << cmd->at(0) << ". I had to kill myself because of YOU!"<<endl;
            exit(EXIT_FAILURE);
        }
        return sml.str();
    } 
    else 
    {
        cerr << "Relop issues on line " << cmd->at(0) << ". I had to kill myself because of YOU!"<<endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Writes the SML for a goto statement
 *
 * @param cmd the pointer ot the line of a comand
 *
 * @return sml as a string
 */
string compiler::_goto(vector<string> *cmd) 
{
    if(cmd->size() == 3) //check that the size of the command ifs valid
    {
      	int linenum;
      	try //attempt to get ye ol lien num
        {
            linenum = stoi(cmd->at(2));
            if(linenum>99 or linenum<0) //out pof bounds line number
            {
                cerr<<"Line pointer out of bounds on line " << cmd->at(0)<<endl;
                exit(EXIT_FAILURE);
            }
            addresses.insert(linenum);
        } 
      	catch(invalid_argument& e) // bad linenum
        {
            cerr<<"Invalid line number on line " << cmd->at(0)<<endl;
            exit(EXIT_FAILURE);
        }
      
      	//save the command to sml
      	stringstream sml;
        sml << "40A" << linenum << endl; //puts temporary value starting with 'a'
      	string ret = sml.str();
        return ret;
    } 
    else //illegal number of arguments to the thingy
    {
      	cerr<<"Invalid number of arguments on line " << cmd->at(0)<<endl;
        exit(EXIT_FAILURE);
    }
}


/**
 * Creates the SML for the let command
 *
 * @param *cmd pointer to the line with the let command
 *
 * @return tuple(sml code, size of command)
*/
tuple<string,int> compiler::let(vector<string> *cmd) {
    if(cmd->size() >= 5 && cmd->at(3) == "=") {
        string final_var = "";
        if(ALPHA.find(cmd->at(2)) != string::npos) {
            final_var = cmd->at(2);
        } else {
            cerr << "Invalid variable on line " << cmd->at(0)<<endl;
			exit(EXIT_FAILURE);
        }
        
        vector<string> infix(cmd->begin()+4, cmd->end()); //with only math part
        vector<string> postfix = to_postfix(infix); //math part to postfix
        
        stringstream sml;
	    string operators="+-/*"; /**/
        stack<string> ids;
        int let_size = 0;
        int local_stack = 0;
        //TODO check if there is only one token and store that to the final var
        //TODO check if size of postfix is 2 because that is wrong
        if(cmd->size() == 5) //only one value (i.e. "10 let x = 9" )
        {
            stringstream sml;
            string final_var = cmd->at(2); //final variable to hold value
            string token = cmd->at(4); //value to be stored in final var
            if(ALPHA.find(token) != string::npos) { //valid variable
                sml << "20" << token << endl; //load variable into acc for moving
                vars.insert(token); //add variable to vars list
            } else { // now it may be a literal but we'll see about that
                int num = 0;
                try {
                    num = stoi(token); 
                } catch(invalid_argument& e) {
                    cerr << "Invalid Argument on line " <<cmd->at(0)<<endl;
	    			exit(EXIT_FAILURE);
                } catch(out_of_range& e) {
                    cerr << "Big thing on line " <<cmd->at(0)<<endl;
	    			exit(EXIT_FAILURE);
                }
                sml << "20C" << to_string(num) << endl; //load constant to acc for moving
                constants.insert(num); //add constant to constants list
            }
            sml << "21" << final_var << endl;
        }
        else if(cmd->size() == 6)
        {
            cerr << "Invalid let command on line " <<cmd->at(0)<<endl;
		    exit(EXIT_FAILURE);
        }
        else
        {
            for(auto i = postfix.begin(); i < postfix.end(); i++) {
                string token = (*i);
                if(operators.find(token) != string::npos) { //pointing to operator
                    string id2 = ids.top();
                    ids.pop();
                    string id1 = ids.top();
                    ids.pop();
                    if(id1[0] == 'S') //if it starts with S, it's popped off stack
                        local_stack--;
                    if(id2[0] == 'S') //if it starts with S, it's popped off stack
                        local_stack--;
                    int operation = 30 + operators.find(token);
                    //uses position plus 30 to convert to SML opcode
                    sml << "20" << id1 << endl //loads id1 into accumulator
                        << operation << id2 << endl; //applies operator on acc with id2 param
                    if(i + 1 == postfix.end()) //last operator so save to final var
                    {
                        sml << 21 << final_var << endl; //store to final var
                    }
                    else
                    {
                        sml << 21 << "S" << local_stack << endl; //else store to stack
                        ids.push("S" + to_string(local_stack)); //push the s to local stack
                        local_stack++; //makes stack 1 larger
                    }
                    
                    let_size += 3; //each operator uses 3 SML ops
                    
                    if(local_stack + 1 > stack_size) //this requires more stack
                        stack_size = local_stack;
                } else { //hopefully pointing to constant or variable
                    if(ALPHA.find(token) != string::npos) { //valid variable
                        ids.push(token);
                        vars.insert(token);
                    } else { // now it may be a literal but we'll see about that
                        int num = 0;
                        try {
                            num = stoi(token);
                        } catch(invalid_argument& e) {
                            cerr << "Invalid Argument on line " <<cmd->at(0)<<endl;
			    			exit(EXIT_FAILURE);
                        } catch(out_of_range& e) {
                            cerr << "Big thing on line " <<cmd->at(0)<<endl;
			    			exit(EXIT_FAILURE);
                        }
                        ids.push("C" + to_string(num));
                        constants.insert(num);
                    }
                }
            }
            return make_tuple(sml.str(), let_size);
        }
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
string compiler::second_parse(string partial_sml) {
    if(program_size + stack_size + vars.size() + constants.size() > 100) {
        cerr << "Program too large." << endl;
    }
    //Replace addresses
    for(auto iter = addresses.begin(); iter != addresses.end(); iter++) {
      	try {
            int replace = address_map.at(*iter);
            string newstr = fmt(to_string(replace), 2, '0');
            partial_sml = replace_all(partial_sml, "A" + to_string(*iter), newstr);
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
        partial_sml = replace_all(partial_sml, "C" + to_string(*iter), newstr)
                + fmt(to_string(*iter), 4, '0') + "\n";
        program_size++;
    }
    
    //Replace stack variables
    for(int i = 0; i < stack_size; i++) {
        string newstr = fmt(to_string(program_size), 2, '0');
        partial_sml = replace_all(partial_sml, "S" + to_string(i), newstr) + "0000\n";
        program_size++; 
    }
    
    //Replace stack variables
    for(string var : vars) {
        string newstr = fmt(to_string(program_size), 2, '0');
        partial_sml = replace_all(partial_sml, var, newstr) + "0000\n";
        program_size++;
    }
  
    return partial_sml;
}

/**
 * Replaces all instances of the old string to the new string in str
 * 
 * @param str String in which replacements will be made
 * @param oldstr String to be replaced
 * @param newstr String to be put in place of oldstr
 * 
 * @return Returns str but with replacements made
 */
string compiler::replace_all(string str, string oldstr, string newstr) 
{
    int oldsize = oldstr.size();
    int strsize = str.size();
    stringstream ret;
    int prev = 0;
    for(int i = 0; i + oldsize <= strsize; i++) 
    {
        if(oldstr == str.substr(i, oldsize)) 
        {
            ret << str.substr(prev, i-prev) << newstr;
            i += oldsize;
            prev = i;
        }
    }
    ret << str.substr(prev, strsize-1);
    return ret.str();
}

/**
 * Formats the string to output with leading zeroes
 *
 * @param original original string
 * @param size how large the string should be at the end
 * @param fill char to fill empty with
 *
 * @return string of string with leading zeroes
*/
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
 * @editor Brennan Cain ADDED THE NEWLINE BRACES
 */
vector<string> compiler::tokenize(string str, string delimiter) 
{
    vector<string> tokens;
    int start = 0;
    int end = 0;
    while (end != string::npos) 
    {
        end = str.find(delimiter, start);
        string sub = str.substr(start, end - start);
        if (sub.size() > 0) 
        {
            tokens.push_back(sub);
        }
        start = end + delimiter.size();
    }
    return tokens;
}
