# TrivialTestKit
A simple C++ library for testing code. 
Allows to create unit tests and execute them with check if they compleated with or without fail.

## HOWTO Run Tests 
To run tests. Select in ***TrivialTestKIt/Test*** project a one of following ***Solution Configuration***:
- ***Release***, for test without definied `WIDE_ORIENTATION` \*;
- ***ReleaseWide***, for test with definied `WIDE_ORIENTATION`;
- ***Debug***, for test without definied `WIDE_ORIENTATION`;
- ***DebugWide***, for test with definied `WIDE_ORIENTATION`.

And then ***Start Without Debugging***.

## Builds and Tests results

| Platform | Compiler | Architecture | Wide Oriented Stream \* | Build | Passing Tests
|-|-|-|-|
Windows 10 | Visual Studio 2020 | x86 | No | <span style="color:green">Success</span> | <span style="color:green">Yes</span>
Windows 10 | Visual Studio 2020 | x86 | Yes | <span style="color:green">Success</span> | <span style="color:green">Yes</span>
Windows 10 | Visual Studio 2020 | x86-64 | No | <span style="color:green">Success</span> | <span style="color:green">Yes</span>
Windows 10 | Visual Studio 2020 | x86-64 | Yes | <span style="color:green">Success</span> | <span style="color:green">Yes</span>
Windows 10 | LLVM GCC 13.0.0 | x86 | No | <span style="color:grey">Not builded</span> | <span style="color:grey">Not Tested</span>
Windows 10 | LLVM GCC 13.0.0 | x86 | Yes | <span style="color:grey">Not builded</span> | <span style="color:grey">Not Tested</span>
Windows 10 | LLVM GCC 13.0.0 | x86-64 | No | <span style="color:green">Success</span> | <span style="color:green">Yes</span>
Windows 10 | LLVM GCC 13.0.0 | x86-64 | Yes | <span style="color:green">Success</span> | <span style="color:green">Yes</span>

<sup>\* When using functions from wprintf family only (those for wide characters, which manipulate any stream).</sup>

