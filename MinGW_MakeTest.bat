@echo off

:: MinGW_MakeTest <action> <mode> <architecture>
::      <action>
::          build
::          run
::      <mode>
::          Debug
::          DebugWIDE
::          Release
::          ReleaseWIDE
::      <architecture>
::          32
::          64

cd TrivialTestKit_Test
call MinGW_Make.bat %*
cd ..