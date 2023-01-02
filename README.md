# GomokuAI
A console Gomoku game application with an AI implemented with Negascout Algorithm. Based on qwertyforce's implementation of depth-limited negascout in C++, found here: https://github.com/qwertyforce/gomoku_ai_c
# Installation Guide

## On a MacOS machine: 
You will need a C++ compiler to compile and run the AI. We are using the one bundled with the XCode Command Line tools. You could install it with

    xcode-select -install

if you have not installed it yet.

Then, open the GomokuAI/src folder in Terminal. Create the object files with:

    g++ -std=c++17 -c main.cpp
    g++ -std=c++17 -c negascoutengine.cpp
    g++ -std=c++17 -c negascoutengine.hpp

Link those object files to compile a single executable file:

    g++ -std=c++17 main.o negascoutengine.o negascoutengine.hpp.gch -o hustAI

Run the AI from Finder or from terminal using

    ./hustAI

You could compile the AI similarly on a Unix-based operating system.

## On a Windows machine:

You will need a C++ compiler to run the AI. You could download MinGW via https://sourceforge.net/projects/mingw/
Run the following command in the Command Prompt to check if installation was done properly:

    g++ --version

The AI is an executable command line program, and is implemented according to the Piskvork new protocol, more on that https://plastovicka.github.io/protocl2en.htm. 

To compile your own AI, run the following command in a terminal opened in GomokuAI/piskvork/src

    g++ engine.cpp pisqpipe.cpp hustAI.cpp -o pbrain-hustAI

Then, run piskvork.exe, go to the Settings option in the Players tab and specify the path to the AI executable.
