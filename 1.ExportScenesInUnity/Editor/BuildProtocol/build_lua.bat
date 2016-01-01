@echo off

set XLS_NAME=%1
set SHEET_NAME=%2
set DATA_DEST=%3



echo.
echo =========Compilation of %XLS_NAME%.xls=========



::---------------------------------------------------
::第一步：把proto翻译成cs
::---------------------------------------------------

set PROTO_PATH=.\Protocol
set STEP1_PROTO2LUA_PATH=.\step1_proto2lua
set PROTO_DESC_POSTFIX=.protodesc
set SRC_OUT=..\src

cd %STEP1_PROTO2LUA_PATH%

@echo off
echo TRY TO DELETE TEMP FILES:
del *.cs
del *.protodesc
del *.txt


@echo on
dir ..\%PROTO_PATH%\*.proto /b  > protolist.txt

@echo on
for /f "delims=." %%i in (protolist.txt) do protoc --proto_path=..\%PROTO_PATH% ..\%PROTO_PATH%\%%i.proto --plugin=protoc-gen-lua="protoc-gen-lua.bat" --lua_out=.\ 

cd..

::---------------------------------------------------
::第二步：将lua拷到Assets里
::---------------------------------------------------

@echo off
set OUT_PATH=..\Assets
set LUA_DEST=StreamingAssets\ScriptsLua\Killer\Protocol\ProtoGen


@echo on
copy %STEP1_PROTO2LUA_PATH%\*.lua %OUT_PATH%\%LUA_DEST%\*.lua.txt

::---------------------------------------------------
::第三步：清除中间文件
::---------------------------------------------------
@echo off
echo TRY TO DELETE TEMP FILES:
cd %STEP1_PROTO2LUA_PATH%
del *.lua
del *.protodesc
del *.txt
cd ..


@echo on