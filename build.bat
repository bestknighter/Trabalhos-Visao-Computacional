@echo off
IF "%~1"=="all" (
    echo.
    echo            Building Release
    call :build_release
    echo.
    echo            Building Debug
    call :build_debug
) ELSE IF "%~1"=="release" (
    call :build_release
) ELSE IF "%~1"=="debug" (
    call :build_debug
) ELSE (
    echo.
    echo build.bat usage:
    echo build release - builds the release version
    echo build debug - builds the debug version
    echo build all - builds both versions
)
goto End

:build_release
set startPath=%cd%
mkdir build
cd ./build
mkdir release
cd ./release
set endPath=%cd%
cmake ../.. -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make
cd ../..
REM robocopy "./resources" "%endPath%/resources" /xn /xo /ns /nc /njs /nfl /ndl /s /e
robocopy "./win-DLLs" "%endPath%" /xn /xo /ns /nc /njs /nfl /ndl /s /e
cd %startPath%
goto :EOF

:build_debug
set startPath=%cd%
mkdir build
cd ./build
mkdir debug
cd ./debug
set endPath=%cd%
cmake ../.. -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
mingw32-make
cd ../..
REM robocopy "./resources" "%endPath%/resources" /xo /xn /ns /nc /njs /nfl /ndl /s /e
robocopy "./win-DLLs" "%endPath%" /xo /xn /ns /nc /njs /nfl /ndl /s /e
cd %startPath%
goto :EOF

:End