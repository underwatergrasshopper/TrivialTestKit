@echo off
setlocal EnableDelayedExpansion

:: <none>, run, clean
set ACTION=%1
if "%ACTION%" equ "" set ACTION=run

:: <none>, Debug, Release, DebugWIDE, ReleaseWIDE
set BUILD_TYPE=%2
if "%BUILD_TYPE%" equ "" set BUILD_TYPE=Debug

:: <none>, 32, 64
set ARCHITECTURE=%3
if "%ARCHITECTURE%" equ "" set ARCHITECTURE=64

if not exist .\\MinGW_MakeCache.bat (
    echo set MINGW32_BIN_PATH=
    echo set MINGW64_BIN_PATH=
) > MinGW_MakeCache.bat 

call .\\MinGW_MakeCache.bat

if "%MINGW32_BIN_PATH%" equ "" goto :SKIP_MINGW32_BIN_PATH
if "%ARCHITECTURE%" neq "32" goto :SKIP_MINGW32_BIN_PATH
set PATH=%MINGW32_BIN_PATH%;%PATH%
:SKIP_MINGW32_BIN_PATH

if "%MINGW64_BIN_PATH%" equ "" goto :SKIP_MINGW64_BIN_PATH
if "%ARCHITECTURE%" neq "64" goto :SKIP_MINGW64_BIN_PATH
set PATH=%MINGW64_BIN_PATH%;%PATH%
:SKIP_MINGW64_BIN_PATH

set PROJECT_FOLDER=
for %%I in (.) do set PROJECT_FOLDER=%%~nxI

set EXE_FILE_NAME=.\\!PROJECT_FOLDER!.exe

set IS_OK=False
if "!BUILD_TYPE!" equ "Debug" set IS_OK=True
if "!BUILD_TYPE!" equ "Release" set IS_OK=True
if "!BUILD_TYPE!" equ "DebugWIDE" set IS_OK=True
if "!BUILD_TYPE!" equ "ReleaseWIDE" set IS_OK=True

set ARCH_PRE=
if "!ARCHITECTURE!" equ "64" set ARCH_PRE=x64
if "!ARCHITECTURE!" equ "32" set ARCH_PRE=Win32

set BUILD_SUB_DIR=..\\build\\mingw_llvm
set BUILD_PATH=!BUILD_SUB_DIR!\\!ARCH_PRE!\\!BUILD_TYPE!
set RETURN_PATH=..\\..\\..\\..\\TrivialTestKit_Test

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
if exist .\\!BUILD_PATH! @rd /S /Q .\\!BUILD_PATH!
exit /b

:CLEAN_ALL
if exist .\\!BUILD_SUB_DIR! @rd /S /Q .\\!BUILD_SUB_DIR!
exit /b

:BUILD
if not exist !BUILD_PATH! md !BUILD_PATH!
cmake -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=!BUILD_TYPE! -D  ARCHITECTURE=!ARCHITECTURE! -S . -B !BUILD_PATH! && cmake --build !BUILD_PATH!
exit /b

:RUN
if not exist !BUILD_PATH! md !BUILD_PATH!
cd !BUILD_PATH!
!EXE_FILE_NAME!
cd !RETURN_PATH!
exit /b

:EOF