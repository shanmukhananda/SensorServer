@echo off
setlocal

echo "-------------------------------------------------------------------------"
echo "~ EXTERNAL DEPENDENCIES ~"
echo "   # git (version control tool used to download packages)"
echo "   # ANDROID_NDK_HOME (Android NDK installation path)"
echo "           - ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake"
echo "           - ANDROID_NDK_HOME/prebuilt/windows-x86_64/bin/make.exe"
echo "   # Visual Studio 2017"
echo "   # CMake"
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
    set "project_dir=%script_dir%\..\.."
    pushd %project_dir%
    set "project_dir=%cd%"
    popd
    call :check_path "%project_dir%"
    goto :eof

:create_directories
    echo DEBUG :create_directories %*
    setlocal
    if not exist "%project_dir%\vendor\downloads" md "%project_dir%\vendor\downloads"
    call :check_path "%project_dir%\vendor\downloads"
    if not exist "%project_dir%\vendor\installed" md "%project_dir%\vendor\installed"
    call :check_path "%project_dir%\vendor\installed"
    if not exist "%project_dir%\vendor\buildtrees" md "%project_dir%\vendor\buildtrees"
    call :check_path "%project_dir%\vendor\buildtrees"
    endlocal
    goto :eof

:download_packages
    echo DEBUG :download_packages %*
    setlocal
    pushd "%project_dir%\vendor\downloads"
    if not exist flatbuffers (
        git clone --recursive https://github.com/google/flatbuffers.git
    )
    popd
    endlocal
    goto :eof

:build_flatbuffer_android
    echo DEBUG :build_flatbuffer_android %*
    setlocal
    set "flatbuffer_dir=%project_dir%\vendor\downloads\flatbuffers"
    call :check_path "%flatbuffer_dir%"

    set "build_type=%1"
    set "cmake_generator=MinGW Makefiles"
    set "toolset=armeabi-v7a"
    set "android_platform=android-16"

    set "android_toolchain=%ANDROID_NDK_HOME%/build/cmake/android.toolchain.cmake"
    call :check_path "%android_toolchain%"

    set "android_make=%ANDROID_NDK_HOME%/prebuilt/windows-x86_64/bin/make.exe"
    call :check_path "%android_make%"

    set "build_dir=%project_dir%\vendor\buildtrees\%toolset%-android-%build_type%"
    set "install_dir=%project_dir%\vendor\installed\%toolset%-android-%build_type%"
    if not exist "%build_dir%" md "%build_dir%"
    call :check_path "%build_dir%"
    if not exist "%install_dir%" md "%install_dir%"
    call :check_path "%install_dir%"

    pushd "%build_dir%"

    cmake -G "%cmake_generator%" ^
    -DANDROID_ABI="%toolset%" ^
    -DANDROID_NATIVE_API_LEVEL="%android_platform%" ^
    -DANDROID_PLATFORM="%android_platform%" ^
    -DCMAKE_BUILD_TYPE="%build_type%" ^
    -DCMAKE_BUILD_TYPE="%build_type%" ^
    -DCMAKE_INSTALL_PREFIX="%install_dir%" ^
    -DCMAKE_MAKE_PROGRAM="%android_make%" ^
    -DCMAKE_TOOLCHAIN_FILE="%android_toolchain%" ^
    -DFLATBUFFERS_BUILD_TESTS=OFF ^
    -DFLATBUFFERS_BUILD_FLATC=OFF ^
    -DFLATBUFFERS_BUILD_FLATHASH=OFF ^
    "%flatbuffer_dir%"

    cmake --build "%build_dir%" --config "%build_type%" --target install -- -j%NUMBER_OF_PROCESSORS%
    endlocal
    goto :eof

:build_flatbuffer_msvc
    echo DEBUG :build_flatbuffer_msvc %*
    setlocal
    set "flatbuffer_dir=%project_dir%\vendor\downloads\flatbuffers"
    call :check_path %flatbuffer_dir%
    set "cmake_generator=Visual Studio 15 2017 Win64"
    set "toolset=msvc-2017-x64"
    set "build_type=%1"
    
    set "build_dir=%project_dir%\vendor\buildtrees\%toolset%-windows-%build_type%"
    set "install_dir=%project_dir%\vendor\installed\%toolset%-windows-%build_type%"
    if not exist "%build_dir%" md "%build_dir%"
    call :check_path "%build_dir%"
    if not exist "%install_dir%" md "%install_dir%"
    call :check_path "%install_dir%"
    pushd "%build_dir%"

    cmake -G "%cmake_generator%" ^
    -DCMAKE_BUILD_TYPE="%build_type%" ^
    -DCMAKE_INSTALL_PREFIX="%install_dir%" ^
    -DFLATBUFFERS_BUILD_TESTS=OFF ^
    -DFLATBUFFERS_BUILD_FLATC=ON ^
    -DFLATBUFFERS_BUILD_FLATHASH=OFF ^
    "%flatbuffer_dir%"
    cmake --build "%build_dir%" --config "%build_type%" --target install
    endlocal
    goto :eof

:build_flatbuffer
    echo DEBUG :build_flatbuffer %*
    setlocal
    call :build_flatbuffer_android release
    call :build_flatbuffer_android debug
    call :build_flatbuffer_msvc release
    call :build_flatbuffer_msvc debug
    popd
    endlocal
    goto :eof

:build_packages
    echo DEBUG :build_packages %*
    setlocal
    call :build_flatbuffer
    endlocal
    goto :eof

:main
    echo DEBUG :main %*
    setlocal
    call :set_project_directory
    call :create_directories
    call :download_packages
    call :build_packages
    endlocal
    goto :eof

endlocal
