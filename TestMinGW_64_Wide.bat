@echo off
setlocal EnableDelayedExpansion

set ACTION=%1
if "%ACTION%" equ "" set ACTION=run

cd Test
call MinGW_Make.bat !ACTION! ReleaseWide 64 C:\llvm-mingw-20220906-ucrt-x86_64\bin
cd ..