@echo off
setlocal

echo "-------------------------------------------------------------------------"
echo "~ EXTERNAL DEPENDENCIES ~"
echo "  # Visual Studio 2017"
echo "      - C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
echo "  # QTDIR (Path containing Qt installation e.g. C:\Qt\Qt5.11.2)"
echo "  # QTKITS (Path containing Qt kits e.g. C:\Qt\Qt5.11.2\5.11.2)"
echo "-------------------------------------------------------------------------"

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

set "project_dir=None"
call :main %*
goto :eof

:check_path
    echo DEBUG :check_path %*
    setlocal
    set "path_to_check=%1"
    if not exist "%path_to_check%" (
        echo "%path_to_check%" does not exist, exiting...
    )
    exit /b %errorcode%
    endlocal
    goto :eof

:set_project_directory
    echo DEBUG :set_project_directory %*
    set "script_dir=%~dp0"
    set "script_dir=%script_dir:~0,-1%"
    set "project_dir=%script_dir%\..\.."
    pushd %project_dir%
    set "project_dir=%cd%"
    popd
    call :check_path "%project_dir%"
    goto :eof

:build
    echo DEBUG :build %*
    setlocal
    set "pro_file=%project_dir%\SensorServer.pro"
    set "jom_path=%QTDIR%\Tools\QtCreator\bin\jom.exe"
    set "qmake_path=%QTKITS%\msvc2017_64\bin\qmake.exe"

    set "toolset=msvc-2017-x64"
    set "build_type=%1"
    set "build_dir=%project_dir%\build\%toolset%-windows-%build_type%"
    if not exist "%build_dir%" md "%build_dir%"

    call :check_path "%pro_file%"
    call :check_path "%jom_path%"
    call :check_path "%qmake_path%"
    call :check_path "%build_dir%"

    pushd "%build_dir%"

    set command="%qmake_path%" "%pro_file%" -spec win32-msvc "CONFIG+=%build_type% qtquickcompiler"
    echo Starting: %command%
    call %command%

    set command="%jom_path%" qmake_all
    echo Starting: %command%
    call %command%

    set command="%jom_path%"
    echo Starting: %command%
    call %command%

    popd
    endlocal
    goto :eof

:main
    echo DEBUG :main %*
    setlocal
    call :set_project_directory
    call :build release
    call :build debug
    endlocal
    goto :eof

endlocal
