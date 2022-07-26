# IakabScript Standard Interpreter (ISSI)
This is the official interpreter implementation for IakabScript.

It is currently under development, as I have to figure out how to properly use the underlying tools and to make a proper language interpreter in C.

## What it does now
See `examples` for some example codes.

ISSI has reached a point where it's easier to list what it _doesn't_ do yet! So ISSI currently implements _the entire_ specification, apart from these points:
* Including files with `avem` is not implemented.
* Arrays are not implemented.
* The `fanumar` builtin function does not accept non-Human formatted strings.

## How it does it
You should know basic interpreter design before diving into the code, but basically, this code does the following:
* Parses the source code and generates a string of tokens.
* Parses the stream of tokens and attempts to figure out the structure of the program.
* As it does that, it also incrementally generates and builds an Abstract Syntax Tree (AST) of the source code (with all the statements, declarations, etc.).
* Starts to parse the AST from its root and executes IS code, handles memory, stack frames, scopes, etc.

The first two bullet points are taken care of by two tools called `flex` and `bison` respectively. They each take one configuration file (collectively, the language definition) and generate C code that will parse IakabScript code and build an AST.

## How to get it running on your machine
You need `flex` and `bison` installed on your machine. You can install them from `apt` or `brew`.

Once done, you can clone the repo, change to the `issi` directory, and run `make`. The output will be at `./issi`. Run the interpreter on some code by running `./issi [filename]`. Errors will be reported to stderr.

You can run `./issi tok` to make the interpreter print the tokens it parses, line by line, and stop there, instead of trying to continue executing the program.

Please note that `./issi tok` might not work properly on MacOS.

