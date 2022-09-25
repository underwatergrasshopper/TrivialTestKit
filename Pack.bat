::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: MIT License
:: 
:: Copyright (c) 2022 underwatergrasshopper
:: 
:: Permission is hereby granted, free of charge, to any person obtaining a copy
:: of this software and associated documentation files (the "Software"), to deal
:: in the Software without restriction, including without limitation the rights
:: to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
:: copies of the Software, and to permit persons to whom the Software is
:: furnished to do so, subject to the following conditions:
:: 
:: The above copyright notice and this permission notice shall be included in all
:: copies or substantial portions of the Software.
:: 
:: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
:: IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
:: FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
:: AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
:: LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
:: OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
:: SOFTWARE.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo off
setlocal EnableDelayedExpansion

where /q 7z.exe || echo Pack Error: Can not find 7z.exe. && exit /b

set VERSION=1.0.0
set NAME=TrivialTestKit
set FILES=README.md CHANGELOG.md LICENSE

set NAME_VERSION=%NAME%-%VERSION%
set INCLUDE_FOLDER=%NAME%\\include

if not exist "dist" mkdir "dist"

echo Clean %NAME_VERSION%
if exist "dist\\%NAME_VERSION%.zip" del "dist\\%NAME_VERSION%.zip"
if exist "dist\\%NAME%" rd /S /Q "dist\\%NAME%"
echo.

echo Pack %NAME_VERSION%
mkdir "dist\\%NAME%"
for %%F in (%FILES%) do xcopy %%F "dist\\%NAME%" /I
if "%INCLUDE_FOLDER%" neq "" (
    xcopy %INCLUDE_FOLDER% "dist\\%NAME%\\include" /I
)


cd dist
call 7z.exe a "%NAME_VERSION%.zip" "%NAME%"
cd ..

if exist "dist\\%NAME%" rd /S /Q "dist\\%NAME%"