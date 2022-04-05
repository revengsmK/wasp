@echo off

echo Building WASP using GCC...

REM Try to use GCC compiler, if available.

gcc -m32 -Wall -I "%~dp0res" "%~dp0src\main.c" "%~dp0src\wasp.c" "%~dp0src\helper.c" "%~dp0res\res.o" -o "%~dp0wasp.exe" -mwindows

echo;

if %errorlevel%==9009 echo GCC is not installed or PATH not properly set! & echo; & goto msvc

if %errorlevel% NEQ 0 ( echo Building failed... & goto end ) else ( echo; & echo Build OK... & goto end )


REM If GCC is not installed, try MSVC compiler

:msvc

echo Building WASP using MSVC compiler... & echo;

cl /I "%~dp0res" "%~dp0src\main.c" "%~dp0src\wasp.c" "%~dp0src\helper.c" "%~dp0res\res.o" /Fo:"%~dp0\" user32.lib gdi32.lib comdlg32.lib /link /out:"%~dp0wasp.exe"

echo;

if %errorlevel%==9009 echo MSVC compiler is not installed or PATH not properly set! & goto end

echo;

if %errorlevel% NEQ 0 (echo; & echo Building failed... & echo;) else (echo; & echo Build OK... & echo;)

REM Clean object files

if exist "%~dp0*.obj" (
echo Deleting object files... 
del "%~dp0\*.obj" 
)

:end

echo;
pause
exit