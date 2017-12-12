@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

set project=dllhook
set libs=user32.lib

cd C:\Users\coderobe\Desktop\dev\%project%\src

cl /std:c++latest /nologo /EHsc /W3 /Ox /fp:fast /Gm- /LD /Fe..\build\%project%.dll /D_CRT_SECURE_NO_DEPRECATE main.cpp %libs% /link /incremental:no

