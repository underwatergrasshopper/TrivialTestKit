@echo off

echo === Build and Test on GCC ===
echo Preparing...
if not exist Release mkdir Relese

echo Clearing... 
if exist Release\\TestGCC.exe del Release\\TestGCC.exe 

:: Custom GCC Compiler
:: set PATH=C:\\MinGW_8_1_0\\bin;%PATH%

echo Building...
set COMMAND=g++ -std=c++11 -O3 -I ./include -I ./../TrivialTestKit/include -I ./src  ./src/*.cpp -o ./Release/TestGCC.exe %*
echo %COMMAND%
call %COMMAND%

echo Building...
if exist Release\\TestGCC.exe (
    echo Running...
    call Release\\TestGCC.exe 
)


