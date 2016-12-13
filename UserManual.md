# User Manual for Simple Basic Compiler
Brennan Cain, Hunter Damron, and James Johnson (he doesnt even go here!)

## Description

Compiler from the Simple Basic language to the Von Neumann simple machine language.

## Usage

### Compile the compiler
Navigate to the root of the compiler folder and execute the command 
```g++ --std=c++11 -o simplec src/main.cpp src/compiler.cpp```

### Compile Simple Basic
The executable can be run using two different configurations for input and output files.

```<simple basic file>``` is the filename for the file containing the Simple Basic source.
```<output file>``` is the file the Von Neumann machine language will be saved to.

```./simplec <simple basic file>```

```./simplec <simple basic file> <output file>```

### Specific Language Issues

This compiler does not save the rem command arguments to the final SML. It can easily be extended by uncommenting the implementation in the compiler.cpp

## Documentation

The compiler documentation is available in the doc directory of the compiler root folder.
