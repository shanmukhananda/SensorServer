# SensorServer

**Mobile application to send Acceleration, Orientation, Geolocation and Images over WebSocket**

## Tools Required

- [Android NDK (Revision 17c)](https://dl.google.com/android/repository/android-ndk-r17c-windows-x86_64.zip)
- [Android SDK](https://dl.google.com/dl/android/studio/install/3.2.1.0/android-studio-ide-181.5056338-windows.exe)
- [CMake (3.12.4)](https://cmake.org/files/v3.12/cmake-3.12.4-win64-x64.msi)
- [Git (2.19.1)](https://github.com/git-for-windows/git/releases/download/v2.19.1.windows.1/Git-2.19.1-64-bit.exe)
- [JDK (8u192)](http://download.oracle.com/otn-pub/java/jdk/8u192-b12/750e1c8617c5452694857ad95c3ee230/jdk-8u192-windows-x64.exe)
- [Qt (5.11.2)](http://download.qt.io/official_releases/qt/5.11/5.11.2/qt-opensource-windows-x86-5.11.2.exe)
- [Visual Studio (2017 Community)](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community)

## Installation

- Visual Studio
    - Launch `vs_community*.exe`
    - Select below **Workloads**
        - *Desktop development with C++*
        - *Linux development with C++*
    - Select below **Individual components**
        - *Git for Windows*
        - *Anaconda3 64-bit (5.2.0)*

- Qt
    - Launch `qt-opensource-windows-x86-5.11.2.exe`
    - Select Below components during component selection
        - `Qt > Qt 5.11.2 > MSVC 2017 64-bit`
        - `Qt > Qt 5.11.2 > Android ARMv7`
        - `Qt > Tools > Qt Creator 4.7.1 CDB Debugger Support`

- Android NDK
    - Extract `android-ndk-r17c-windows-x86_64.zip` to any folder
    - Example `C:\Android\android-ndk-r17c`, sub-folders like `prebuilt`, `sysroot` should come under this directory

- Android SDK
    - Extract `sdk-tools-windows*.zip` to any folder
    - Example `C:\Android\sdk-tools-windows`, sub-folders like `tools` should come under this directory
    - Launch Command Prompt on this extracted directory `C:\Android\sdk-tools-windows`
    - Run `tools\bin\sdkmanager.bat "platform-tools" "platforms;android-28" "build-tools;28.0.3"`

## Update Environment Variables

- `Path`
    - Below executable's directories are required to be appended to `Path`
        - `git.exe` *`(C:\Program Files\Git\cmd)`*
        - `cmake.exe` *`(C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin)`*
        - `clang-format.exe` *`(C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\vcpackages)`*
        - `java.exe` *`(C:\Program Files\Java\jdk1.8.0_192\bin)`*
- `ANDROID_SDK_HOME=C:\Android\sdk-tools-windows`
- `ANDROID_NDK_HOME=C:\Android\android-ndk-r17c`
- `QTDIR=C:\Qt\Qt5.11.2`
- `QTKITS=C:\Qt\Qt5.11.2\5.11.2`
- `JAVA_HOME=C:\Program Files\Java\jdk1.8.0_192`

## Build

Steps to Build for Windows and Android

1. Build third party dependencies using `scripts\windows\bootstrap.bat`
2. Generate IDL source code using `scripts\windows\detail\generate.bat`
3. Build for MSVC 2017 target using `scripts\windows\build_msvc.bat` or build for Android using `scripts\windows\build_android.bat`

## Miscellaneous

- Clang Code format `scripts\windows\detail\code_format.bat`
