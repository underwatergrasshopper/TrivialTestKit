@echo off

cd Test
call MinGW_Make.bat run Release 64 C:\llvm-mingw-20220906-ucrt-x86_64\bin
cd ..