SIMPLE-Compiler
===============
### Compiler for SIMPLE (a BASIC dialect) into SML (an interpreted language similar to machine code). 

# Compile
Compile using predefined Makefile for Linux or with the following
```
g++ --std=c++11 -o simplec src/main.cpp src/compiler.cpp
```

For Windoze users, compile using preferred C++ compiler

# Execute

Compile a file "input.sb" by typing the following
```
./simplec <input file>
```
or
```
./simplec <input file> <output file>
```
SML code can be interpreted using the [SML Interpreter](https://github.com/hdamron17/SML-Interpreter) on Github

=========
See User manual for more

Project hosted on [Github](https://github.com/hdamron17/SIMPLE-Compiler)
