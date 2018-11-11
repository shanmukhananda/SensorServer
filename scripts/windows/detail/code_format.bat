@echo off
setlocal

set "project_dir=None"
call :main %*
goto :eof

:clang_format_file
    REM echo DEBUG :clang_format_file %*
    setlocal
    set "source_file=%1"
    set "command=clang-format -i -style=file %source_file%"
    REM echo Starting: %command%
    call %command%
    endlocal
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

:clang_format_dir
    echo DEBUG :clang_format_dir %*
    setlocal
    set "input_dir=%1"
    call :check_path %input_dir%
    set source_files_ext=*.cpp *.hpp *.c *.h *.cc *.hh

    pushd %input_dir%
    set command=dir /s /b %source_files_ext%
    for /f "delims=" %%a in ('%command%') do (
         call :clang_format_file %%a
    )

    popd
    endlocal
    goto :eof
    
:clang_format_all
    echo DEBUG :clang_format_all %*
    setlocal
    call :clang_format_dir %project_dir%\app
    call :clang_format_dir %project_dir%\experiments
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
    echo DEBUG :main %*
    call :set_project_directory
    pushd %project_dir%
    call :clang_format_all
    popd
    goto :eof

endlocal
