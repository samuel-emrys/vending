# README

Personal Note: This was the second programming project I undertook. The general structure for the program was provided by Paul Miller, however the implementation of the program is my own. This project was completed to satisfy the course requirements of COSC1076 Advanced Programming Techniques at RMIT University in Semester 2 of 2015.

# vending
 
## Compiling
---------------------------------------------------------------------------
In order to compile the source files into an executable, a makefile has been 
provided, and the following command should be run in terminal from the project
root: 

	$ make

This will create an executable which can then be opened in terminal using
the command

	./ppd <coinfile> <stockfile>

As an example, from the project root, execute:

	./bin/ppd dat/coins.dat dat/stock.dat

A range of other options including "debug" and "clean" can also be used in
order to compile it in a debug configuration and also to remove all extraneous
files from the folder, respectively using the following commands:

	$ make debug
	$ make clean

## Known Bugs
---------------------------------------------------------------------------
1. Different behaviour on different operating systems. 
Specifically this refers to the difference between the way gcc compiles on
the RMIT coreteaching servers and on Mac OSX. This difference affects the way
that the Ctrl-D character is read by the executable, and varies the need to
clear the input buffer after Ctrl-D has been pressed. In OSX, there is a 
requirement to clear the buffer otherwise the game gets stuck in an infinite 
loop. This is not the case on the coreteaching servers, and so the 
"readrestofline()" function has been removed to ensure proper functionality 
for submission at the relevant stage.