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

set IS_OK=False
if "!BUILD_TYPE!" equ "Debug" set IS_OK=True
if "!BUILD_TYPE!" equ "Release" set IS_OK=True
if "!BUILD_TYPE!" equ "DebugWide" set IS_OK=True
if "!BUILD_TYPE!" equ "ReleaseWide" set IS_OK=True

set ARCH_PRE=
if "!ARCHITECTURE!" equ "64" set ARCH_PRE=x64\\

set BUILD_PATH=Build\\MinGW_LLVM\\!ARCH_PRE!!BUILD_TYPE!
set RETURN_PATH=..\\..\\..
if "!ARCHITECTURE!" equ "64" set RETURN_PATH=!RETURN_PATH!\\..



if "!IS_OK!" equ "True" (
    if "!ACTION!" equ "build" (
        call :BUILD
    ) else if "!ACTION!" equ "rebuild" (
        call :CLEAN
        call :BUILD
    ) else if "!ACTION!" equ "clean" (
        call :CLEAN
    ) else if "!ACTION!" equ "clean_all" (
        call :CLEAN_ALL
    ) else if "!ACTION!" equ "run" (
        call :BUILD
        call :RUN
    ) else (
        echo Run Error: Unknown action type: "!ACTION!".
    )
    
) else (
    echo Run Error: Unknown build type: "!BUILD_TYPE!".
)

goto :EOF


:CLEAN
set BUILD_PATH=.\\Build\\MinGW_LLVM\\!ARCH_PRE!!BUILD_TYPE!
if exist !BUILD_PATH! @rd /S /Q !BUILD_PATH!
exit /b

:CLEAN_ALL
if exist .\\Build\\MinGW_LLVM @rd /S /Q .\\Build\\MinGW_LLVM
exit /b

:BUILD
if not exist !BUILD_PATH! md !BUILD_PATH!
cd !BUILD_PATH!
cmake -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=!BUILD_TYPE! -D  ARCHITECTURE=!ARCHITECTURE! !RETURN_PATH! && cmake --build .
cd !RETURN_PATH!
exit /b

:RUN
if not exist !BUILD_PATH! md !BUILD_PATH!
cd !BUILD_PATH!
!EXE_FILE_NAME!
cd !RETURN_PATH!
exit /b

:EOF