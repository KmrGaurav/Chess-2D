@echo off

IF NOT EXIST ..\..\Build mkdir ..\..\Build
pushd ..\..\Build
cl -nologo -Zi ..\Chess\code\win32_chess.cpp user32.lib gdi32.lib
popd