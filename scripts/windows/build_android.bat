@echo off
setlocal

echo "-------------------------------------------------------------------------"
echo "~ EXTERNAL DEPENDENCIES ~"
echo "  # ANDROID_NDK_HOME (Android NDK installation path)"
echo "      - ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake"
echo "      - ANDROID_NDK_HOME/prebuilt/windows-x86_64/bin/make.exe"
echo "  # Visual Studio 2017"
echo "  # QTDIR (Path containing Qt installation e.g. C:\Qt\Qt5.11.2\Qt5.11.2)"
echo "  # QTKITS (Path containing Qt kits e.g. C:\Qt\Qt5.11.2\5.11.2)"
echo "  # ANDROID_SDK_ROOT (Android SDK installation path)"
echo "  # JAVA_HOME (JDK Path e.g C:\Program Files\Java\jdk1.8.0_192)"
echo "-------------------------------------------------------------------------"

set "project_dir=None"
call :main %*
goto :eof

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
    echo DEBUG :set_project_directory %*
    set "script_dir=%~dp0"
    set "script_dir=%script_dir:~0,-1%"
    set "project_dir=%script_dir%\..\.."
    pushd %project_dir%
    set "project_dir=%cd%"
    popd
    call :check_path "%project_dir%"
    goto :eof

:deploy
    echo DEBUG :deploy %*
    setlocal
    set "build_dir=%1"
    set "install_dir=%2"
    set "android_deployer=%QTKITS%\android_armv7\bin\androiddeployqt.exe"
    set "deployment_settings=%build_dir%/android-libSensorServer.so-deployment-settings.json"
    set "platform=android-28"
    set jdk_path=%JAVA_HOME%
    
    set "android_deployer=%android_deployer:\=/%"
    set "deploy_dir=%deploy_dir:\=/%"
    set "jdk_path=%jdk_path:\=/%"

    call :check_path "%android_deployer%"
    call :check_path "%deployment_settings%"

    set "command="%android_deployer%" --input "%deployment_settings%" --output "%install_dir%" --android-platform %platform% --jdk "%jdk_path%" --gradle"
    echo Starting: %command%
    call %command%
    endlocal
    goto :eof

:build
    echo DEBUG :build %*
    setlocal
    set "pro_file=%project_dir%\SensorServer.pro"
    set "ndk_make=%ANDROID_NDK_HOME%\prebuilt\windows-x86_64\bin\make.exe"
    set "ANDROID_NDK_ROOT=%ANDROID_NDK_HOME%"
    set "qmake_path=%QTKITS%\android_armv7\bin\qmake.exe"
    set "toolset=armeabi-v7a"
    set "build_type=%1"
    set "build_dir=%project_dir%\build\%toolset%-android-%build_type%"
    set "install_dir=%project_dir%\installed\%toolset%-android-%build_type%"
    if not exist "%build_dir%" md "%build_dir%"
    if not exist "%install_dir%" md "%install_dir%"

    set "qmake_path=%qmake_path:\=/%"
    set "pro_file=%pro_file:\=/%"
    set "ndk_make=%ndk_make:\=/%"
    set "ANDROID_NDK_ROOT=%ANDROID_NDK_ROOT:\=/%"
    SET "ANDROID_NDK_HOST=windows-x86_64"

    call :check_path "%pro_file%"
    call :check_path "%ndk_make%"
    call :check_path "%qmake_path%"
    call :check_path "%build_dir%"

    pushd "%build_dir%"

    set command="%qmake_path%" "%pro_file%" -spec android-g++ "CONFIG+=%build_type% qtquickcompiler"
    echo Starting: %command%
    call %command%

    set command="%ndk_make%" qmake_all
    echo Starting: %command%
    call %command%

    set command="%ndk_make%" -j%NUMBER_OF_PROCESSORS%
    echo Starting: %command%
    call %command%

    set command="%ndk_make%" INSTALL_ROOT="%install_dir%" install
    echo Starting: %command%
    call %command%

    REM call :deploy "%build_dir%" "%install_dir%"

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
