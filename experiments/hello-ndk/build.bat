@echo off

setlocal

set project_dir=%~dp0


set "android_toolchain=%ANDROID_NDK_HOME%/build/cmake/android.toolchain.cmake"
set "android_platform=android-23"
set "android_make=%ANDROID_NDK_HOME%/prebuilt/windows-x86_64/bin/make.exe"
set "cmake_generator=MinGW Makefiles"
set "abi=armeabi-v7a"
set "build_type=release"

set "build_dir=%project_dir%\products\%android_platform%-%abi%-%build_type%"

mkdir "%build_dir%"
cd "%build_dir%"

cmake -G "%cmake_generator%" ^
-DCMAKE_TOOLCHAIN_FILE="%android_toolchain%" ^
-DCMAKE_MAKE_PROGRAM="%android_make%" ^
-DANDROID_NATIVE_API_LEVEL="%android_platform%" ^
-DANDROID_PLATFORM="%android_platform%" ^
-DANDROID_ABI="%abi%" ^
-DCMAKE_BUILD_TYPE="%build_type%" ^
"%project_dir%"

cmake --build "%build_dir%" --config "%build_type%"
endlocal
