rd /s /q "proj\visualstudio\engine\include"
mklink /J proj\visualstudio\engine\include include\engine
rd /s /q "proj\visualstudio\engine\src"
mklink /J proj\visualstudio\engine\src engine
rd /s /q "proj\visualstudio\game\src"
mklink /J proj\visualstudio\game\src src
rd /s /q "proj\visualstudio\game\include"
mklink /J proj\visualstudio\game\include include\Foundation
rd /s /q "proj\visualstudio\support\murmurhash\include"
mklink /J proj\visualstudio\support\murmurhash\include include\murmurhash
rd /s /q "proj\visualstudio\support\murmurhash\src"
mklink /J proj\visualstudio\support\murmurhash\src lib\murmurhash
rd /s /q "proj\visualstudio\support\tinyxml2\include"
mklink /J proj\visualstudio\support\tinyxml2\include include\tinyxml2
rd /s /q "proj\visualstudio\support\tinyxml2\src"
mklink /J proj\visualstudio\support\tinyxml2\src lib\tinyxml2
call %windir%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe proj\support_vs11.sln /p:Configuration=Debug
call %windir%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe proj\support_vs11.sln /p:Configuration=Release
pause