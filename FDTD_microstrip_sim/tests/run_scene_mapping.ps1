# Run in a Visual Studio x64 Native Tools PowerShell after building Release.
# This integration test needs a working desktop OpenGL context (window is hidden).
param(
    [string]$ObjectDirectory = "$PSScriptRoot/../x64/Release/p0-obj",
    [string]$LibraryRoot = "C:/Users/A/Documents/Libraries"
)
$ErrorActionPreference = 'Stop'
$project = (Resolve-Path "$PSScriptRoot/..").Path
$output = [IO.Path]::GetFullPath("$project/../x64/validation")
New-Item -ItemType Directory -Force -Path $output | Out-Null
$objects = @(Get-ChildItem -LiteralPath $ObjectDirectory -Filter '*.obj' |
    Where-Object Name -ne 'main.obj' | ForEach-Object FullName)
if ($objects.Count -eq 0) { throw 'Build the application first; no object files found.' }
$compileArgs = @('/nologo', '/std:c++14', '/EHsc', '/MD', '/O2',
    "/I$LibraryRoot/glfw-3.4.bin.WIN64/include", "/I$LibraryRoot/glad/include", "/I$LibraryRoot/glm",
    "$PSScriptRoot/scene_mapping.cpp", "/Fe:$output/scene_mapping.exe", "/Fo:$output/scene_mapping.obj")
$compileArgs += $objects
$compileArgs += @('/link', '/LTCG', "/LIBPATH:$LibraryRoot/glfw-3.4.bin.WIN64/lib-vc2022",
    'glfw3.lib', 'opengl32.lib', 'user32.lib', 'gdi32.lib', 'shell32.lib')
& cl @compileArgs
if ($LASTEXITCODE -ne 0) { throw 'Scene mapping test compilation failed.' }
Push-Location $project
try {
    & "$output/scene_mapping.exe"
    if ($LASTEXITCODE -ne 0) { throw 'Scene mapping integration test failed.' }
} finally { Pop-Location }
