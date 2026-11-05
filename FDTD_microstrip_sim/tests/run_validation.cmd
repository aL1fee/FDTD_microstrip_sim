@echo off
rem Run from a Visual Studio x64 Native Tools command prompt.
setlocal
where cl >nul 2>nul
if errorlevel 1 (
  echo Open a Visual Studio x64 Native Tools command prompt first.
  exit /b 1
)
set "output=%~dp0..\..\x64\validation"
if not exist "%output%" mkdir "%output%"
pushd "%output%"
for %%T in (field_probes scene_sources field_views grid_settings length_units yee3d_cavity yee3d_interface yee3d_materials yee3d_conductors cavity_controller) do (
  cl /nologo /std:c++14 /W4 /EHsc /O2 "%~dp0%%T.cpp" /Fe:%%T.exe
  if errorlevel 1 goto fail
  %%T.exe
  if errorlevel 1 goto fail
)
popd
exit /b 0
:fail
popd
exit /b 1
