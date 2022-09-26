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

echo === Build and Test on GCC ===
call !CMP! -v

cd Test

echo Preparing...
if not exist %FOLDER% mkdir %FOLDER%

echo Clearing... 
if exist %FOLDER%\\Test.exe del %FOLDER%\\Test.exe 

echo Building...

set FLAGS=-std=c++11 -O3
if "%PLATFORM%" equ "32" (
    set FLAGS=!FLAGS! -m32
) else if "%PLATFORM%" equ "64" (
    set FLAGS=!FLAGS! -m64
) 

set DEFINES=
if "%CONFIGURATION%" equ "ReleaseWide" (
    set DEFINES=!DEFINES! -D WIDE_ORIENTED
)

set INCLUDES=
set INCLUDES=!INCLUDES! -I ./../TrivialTestKit/include
set INCLUDES=!INCLUDES! -I ./include
set INCLUDES=!INCLUDES! -I ./src
set INCLUDES=!INCLUDES! -I ./src/Folderф
set INCLUDES=!INCLUDES!

set SOURCES=
set SOURCES=!SOURCES! ./src/*.cpp

set OUTPUT=
set OUTPUT=!OUTPUT! ./%FOLDER%/Test.exe

set COMMAND=!CMP! !FLAGS! !DEFINES! !INCLUDES! !SOURCES! -o !OUTPUT!
echo %COMMAND%
call %COMMAND%

if exist %FOLDER%\\Test.exe (
    echo Running...
    call %FOLDER%\\Test.exe
)

cd ..




