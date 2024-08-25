cd %~dp0

mkdir .\Output\x64-Debug

xcopy /y /q .\External\OpenUI\External\x64-windows\debug\bin\*.dll .\Output\x64-Debug

xcopy /y /q .\External\OpenUI\External\x64-windows\debug\bin\*.pdb .\Output\x64-Debug

xcopy /y /q .\External\x64-windows\debug\bin\*.dll .\Output\x64-Debug

xcopy /y /q .\External\x64-windows\debug\bin\*.pdb .\Output\x64-Debug

mkdir .\Output\x64-Release

xcopy /y /q .\External\OpenUI\External\x64-windows\bin\*.dll .\Output\x64-Release

xcopy /y /q .\External\OpenUI\External\x64-windows\bin\*.pdb .\Output\x64-Release

xcopy /y /q .\External\x64-windows\bin\*.dll .\Output\x64-Release

xcopy /y /q .\External\x64-windows\bin\*.pdb .\Output\x64-Release