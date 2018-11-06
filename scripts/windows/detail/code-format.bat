@echo off
setlocal

echo "-------------------------------------------------------------------------"
echo "~ EXTERNAL DEPENDENCIES ~"
echo "   # Enable WSL (Windows Subsystem for Linux)"
echo "       - Start > Turn Windows features on or Off > Windows Subsystem for Linux"
echo "   # Ubuntu App"
echo "       - Start > Microsoft Store > Search and Install, "Ubuntu""
echo "       - https://www.microsoft.com/store/productId/9NBLGGH4MSV6"
echo "-------------------------------------------------------------------------"

set "project_dir=None"
call :main
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
    echo DEBUG :check_path %*
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
    bash -c "scripts/linux/detail/code-format.sh"
    popd
    goto :eof

endlocal
