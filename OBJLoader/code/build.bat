@echo off

IF NOT EXIST ..\dll mkdir ..\dll
pushd ..\dll

set commonCompilerFlags=-nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DOBJ_INTERNAL=1 -DOBJ_SLOW=1 -FC -Zi 

set commonLinkerFlags= -incremental:no user32.lib gdi32.lib winmm.lib

cl %commonCompilerFlags% ..\code\directx_obj_loader.cpp  /LD /link /EXPORT:DirectXLoadOBJ

popd
