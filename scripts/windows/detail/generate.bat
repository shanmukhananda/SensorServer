@echo off
setlocal

echo "-------------------------------------------------------------------------"
echo "~ EXTERNAL DEPENDENCIES ~"
echo "   # vendor\installed\msvc-2017-x64-windows-release\bin\flatc.exe"
echo "          - scripts\windows\bootstrap.bat should create above file"
echo "-------------------------------------------------------------------------"

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
    set "project_dir=%script_dir%\..\..\.."
    pushd %project_dir%
    set "project_dir=%cd%"
    popd
    call :check_path "%project_dir%"
    goto :eof

:main
    setlocal
    call :set_project_directory
    set "flatc_path=%project_dir%\vendor\installed\msvc-2017-x64-windows-release\bin"
    call :check_path "%flatc_path%"
    set "path=%flatc_path%;%path%"
    pushd "%project_dir%\app\messages"
    set command=flatc --cpp %project_dir%\app\messages\messages.fbs
    echo Starting: %command%
    call %command%

    set command=flatc --cpp %project_dir%\app\messages\udpheader.fbs
    echo Starting: %command%
    call %command%
    popd
    endlocal

endlocal
