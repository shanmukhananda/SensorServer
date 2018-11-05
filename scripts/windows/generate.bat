@echo off
setlocal

set "project_dir=%~dp0..\.."

echo Generating...
set "flatc_path=%project_dir%\vendor\installed\msvc-2017-x64-windows-release\bin"
set "path=%flatc_path%;%path%"

pushd "%project_dir%\app\messages"
flatc --cpp %project_dir%\app\messages\messages.fbs
popd

endlocal
