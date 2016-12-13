SIMPLE-Compiler
===============
### Compiler for SIMPLE (a BASIC dialect) into SML (an interpreted language similar to machine code). 

# Compile
Compile using predefined Makefile for Linux or with the following
```
g++ --std=c++11 -o <destination> src/main.cpp src/compiler.cpp
```

For Windoze users, compile using preferred C compiler

# Execute

Execute a file "input.sb" by typing the following
```
./<executable> input.sb
```
