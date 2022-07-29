# IakabScript Standard Interpreter (ISSI)
This is the official interpreter implementation for IakabScript.

It is currently under development, as I have to figure out how to properly use the underlying tools and to make a proper language interpreter in C.

## What it does now
See `in.txt` for some example code (i use that for testing).
* Supports number, string and `nui` literals.
* It can parse top-level variable declarations and assignments.
* Prints in the console, very verbosely, what it's doing.
* Complains and stops immediately on any sort of error.
* Throws errors when variables are redeclared, or when they are referenced but not previously declared.
* Supports `fa..gata` blocks with any nesting level.

## How it does it
You should know basic interpreter design before diving into the code, but basically, this code does the following:
* Parses the source code and generates a string of tokens.
* Parses the stream of tokens and attempts to figure out the structure of the program.
* As it does that, it also incrementally generates and builds an Abstract Syntax Tree (AST) of the source code (with all the statements, declarations, etc.).
* Starts to parse the AST from its root and executes IS code, handles memory, stack frames, scopes, etc.

The first two bullet points are taken care of by two tools called `flex` and `bison` respectively. They each take one configuration file (collectively, the language definition) and generate C code that will parse IakabScript code and build an AST.

## How to get it running on your machine
You need `flex` and `bison` installed on your machine. You can install them from `apt` or `brew`.

Once done, you can clone the repo, change to the `issi` directory, and run `make`. The output will be at `./issi`, and it expects IS code from the standard input (ie. run the test code with `./issi < in.txt`).

You can run `./issi tok` to make the interpreter print the tokens it parses, line by line, and stop there, instead of trying to continue executing the program.

Please note that `./issi tok` might not work properly on MacOS.

## Immediate todos
* Scope variables to their own blocks (disallow cross-block referencing)
