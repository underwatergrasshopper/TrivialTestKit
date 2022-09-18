# TrivialTestKit
A simple C++ library for unit testing code.

## Builds and Tests

| Platform | Compiler | Wide Oriented  Stream \* | Passing Build | Passing Tests
|-|-|-|-|
Windows 10 | Visual Studio 2020 | No | <span style="color:green">Yes</span> | <span style="color:green">Yes</span>
Windows 10 | Visual Studio 2020 | Yes | <span style="color:green">Yes</span> | <span style="color:green">Yes</span>
Windows 10 | LLVM GCC 13.0.0 | No | <span style="color:green">Yes</span> | <span style="color:green">Yes</span>
Windows 10 | LLVM GCC 13.0.0 | Yes | <span style="color:green">Yes</span> | <span style="color:red">No</span>

<sup>\* Using functions from wprintf family only (those for wide characters, which manipulate any stream).</sup>