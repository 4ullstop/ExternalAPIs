@echo off

IF NOT EXIST ..\dll mkdir ..\dll
pushd ..\dll

set commonCompilerFlags=-nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4101 -DTEST_INTERNAL=1 -DTEST_SLOW=1 -DTEST_WIN32=1 -FC -Zi 

set commonLinkerFlags=-incremental:no user32.lib gdi32.lib winmm.lib d3d11.lib dxgi.lib

cl %commonCompilerFlags% ..\code\win32_framework.cpp /LD /link %commonLinkerFlags% /EXPORT:Win32ProcessPendingMessages /EXPORT:Win32LoadGameCode /EXPORT:CheckAndLoadGameCode /EXPORT:Win32GameCodeSetup /EXPORT:Win32CreateSpawnableBuffers /EXPORT:FromV4ToXMVECTOR /EXPORT:ConvertGameCameraDataToWin32

popd
