@echo off

IF NOT EXIST ..\dll mkdir ..\dll
pushd ..\dll

set commonCompilerFlags=-nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4101 -DTEST_INTERNAL=1 -DTEST_SLOW=1 -DTEST_WIN32=1 -DUSE_FORTY_MATH_FAST=1 -FC -Zi 

set commonLinkerFlags=-incremental:no user32.lib gdi32.lib winmm.lib d3d11.lib dxgi.lib

cl %commonCompilerFlags% ..\code\game_framework.cpp /LD /link /EXPORT:CreateViewAndPerspective /EXPORT:GameUpdateCamera /EXPORT:LoadGameOBJFiles /EXPORT:SpawnNewOBJ %commonLinkerFlags% 

popd
