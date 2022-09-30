@echo off
setlocal EnableDelayedExpansion

:: <none>, run, clean
set ACTION=%1
if "%ACTION%" equ "" set ACTION=run

:: <none>, Debug, Release, DebugWide, ReleaseWide
set BUILD_TYPE=%2
if "%BUILD_TYPE%" equ "" set BUILD_TYPE=Debug

:: <none>, 32, 64
set ARCHITECTURE=%3
if "%ARCHITECTURE%" equ "" set ARCHITECTURE=64

:: <none>, URL
set CMP_DIR=%4

if "%CMP_DIR%" equ "" goto :SKIP
set PATH=%CMP_DIR%;%PATH%
:SKIP

set PROJECT_FOLDER=
for %%I in (.) do set PROJECT_FOLDER=%%~nxI

set EXE_FILE_NAME=.\\!PROJECT_FOLDER!.exe

if "!ACTION!" equ "run" (
    set IS_OK=False
    if "!BUILD_TYPE!" equ "Debug" set IS_OK=True
    if "!BUILD_TYPE!" equ "Release" set IS_OK=True
    if "!BUILD_TYPE!" equ "DebugWide" set IS_OK=True
    if "!BUILD_TYPE!" equ "ReleaseWide" set IS_OK=True
    
    set ARCH_PRE=
    if "!ARCHITECTURE!" equ "64" set ARCH_PRE=x64\\
    
    if "!IS_OK!" equ "True" (
        set BUILD_PATH=Build\\MinGW_LLVM\\!ARCH_PRE!!BUILD_TYPE!
        set RETURN_PATH=..\\..\\..
        if "!ARCHITECTURE!" equ "64" set RETURN_PATH=!RETURN_PATH!\\..
        
        if not exist !BUILD_PATH! md !BUILD_PATH!
        cd !BUILD_PATH!
        cmake -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=!BUILD_TYPE! -D  ARCHITECTURE=!ARCHITECTURE! !RETURN_PATH! && cmake --build . && !EXE_FILE_NAME!
        cd !RETURN_PATH!
    ) else (
        echo Run Error: Unknown build type: "!BUILD_TYPE!".
    )
) else if "!ACTION!" equ "clean" (
    if exist .\\Build\\MinGW_LLVM @rd /S /Q .\\Build\\MinGW_LLVM
) else (
    echo Run Error: Unknown action type: "!ACTION!".
)