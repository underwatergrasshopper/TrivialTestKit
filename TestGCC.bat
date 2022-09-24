@echo off
@chcp 65001
setlocal EnableDelayedExpansion

:: Release, ReleaseWide
set CONFIGURATION=%1

:: 32, 64
set PLATFORM=%2

set FOLDER=%CONFIGURATION%GCC

:: For local GCC Compiler
:: set PATH=C:\\MinGW_15_0_0_llvm-mingw\\bin;%PATH%

echo === Build and Test on GCC ===
cd Test

echo Preparing...
if not exist %FOLDER% mkdir %FOLDER%

echo Clearing... 
if exist %FOLDER%\\Test.exe del %FOLDER%\\Test.exe 

set DEFINES=
if "%CONFIGURATION%" equ "ReleaseWide" (
    set DEFINES=-D WIDE_ORIENTED
)

set ARCH=-m32
if "%PLATFORM%" equ "64" (
    set ARCH=-m64
) 

echo Building...
set COMMAND=g++ -std=c++11 -O3 !ARCH! !DEFINES! -I ./../TrivialTestKit/include -I ./include -I ./src -I ./src/Folderф ./src/*.cpp -o ./%FOLDER%/Test.exe
echo %COMMAND%
call %COMMAND%

if exist %FOLDER%\\Test.exe (
    echo Running...
    call %FOLDER%\\Test.exe
)

cd ..




