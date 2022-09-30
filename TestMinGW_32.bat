@echo off

cd Test
call MinGW_Make.bat run Release 32 C:\llvm-mingw-20220906-ucrt-i686\bin
cd ..