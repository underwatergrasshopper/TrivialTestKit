@echo off
@chcp 65001 > nul
setlocal EnableDelayedExpansion

:: Release, ReleaseWide
set CONFIGURATION=%1

:: 32, 64, -
set PLATFORM=%2

:: g++, ...
set CMP=%3

if "%CMP%" equ "" (
    set CMP=g++
)

set FOLDER=%CONFIGURATION%GCC

:: For local GCC Compiler
:: set PATH=C:\\MinGW_15_0_0_llvm-mingw\\bin;%PATH%

echo === Build and Test on GCC ===
call !CMP! -v

cd Test

echo Preparing...
if not exist %FOLDER% mkdir %FOLDER%

echo Clearing... 
if exist %FOLDER%\\Test.exe del %FOLDER%\\Test.exe 

set DEFINES=
if "%CONFIGURATION%" equ "ReleaseWide" (
    set DEFINES=-D WIDE_ORIENTED
)

set ARCH=
if "%PLATFORM%" equ "32" (
    set ARCH=-m32
) else if "%PLATFORM%" equ "64" (
    set ARCH=-m64
) 

echo Building...
set COMMAND=!CMP! -std=c++11 -O3 !ARCH! !DEFINES! -I ./../TrivialTestKit/include -I ./include -I ./src -I ./src/Folderф ./src/*.cpp -o ./%FOLDER%/Test.exe
echo %COMMAND%
call %COMMAND%

if exist %FOLDER%\\Test.exe (
    echo Running...
    call %FOLDER%\\Test.exe
)

cd ..




