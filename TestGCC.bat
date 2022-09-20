@echo off
setlocal EnableDelayedExpansion

:: For local GCC Compiler
:: set PATH=C:\\MinGW_8_1_0\\bin;%PATH%

echo === Build and Test on GCC ===
cd Test

echo Preparing...
if not exist Release mkdir Relese

echo Clearing... 
if exist Release\\TestGCC.exe del Release\\TestGCC.exe 

set DEFINES=
if "%1" equ "wide" (
    set DEFINES=-D WIDE_ORIENTED
)

echo Building...
set COMMAND=g++ -std=c++11 -O3 !DEFINES! -I ./../TrivialTestKit/include -I ./include -I ./src ./src/*.cpp -o ./Release/TestGCC.exe
echo %COMMAND%
call %COMMAND%

echo Building...
if exist Release\\TestGCC.exe (
    echo Running...
    call Release\\TestGCC.exe
)

cd ..




