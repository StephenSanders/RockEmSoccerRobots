@echo off
title publish

call %windir%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe ..\proj\game_vs11.sln /p:Configuration=Release

rd /s /q "..\archive"

mkdir ..\archive
mkdir ..\archive\bin
copy ..\bin\game.exe "..\archive\bin\Rock Em' Soccer Robots.exe"
xcopy ..\data ..\archive\data\ /s /e

exit
