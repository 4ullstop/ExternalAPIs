@echo off

IF NOT EXIST ..\dll mkdir ..\dll
pushd ..\dll

set commonCompilerFlags=-nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DTEST_INTERNAL=1 -DTEST_SLOW=1 -DTEST_WIN32=1 -FC -Zi 

set commonLinkerFlags= -incremental:no user32.lib gdi32.lib winmm.lib

cl %commonCompilerFlags% ..\code\memory_pools.cpp /LD /link /EXPORT:PushStruct /EXPORT:PushArray /EXPORT:PoolAlloc /EXPORT:InitializeArena /EXPORT:ClearArena /EXPORT:PushArraySized

popd
