set PROTO_PATH=.\Protos
set CPP_OUT=..\ProtoGeneratedScrips

protoc.exe -I=%PROTO_PATH% --cpp_out=%CPP_OUT% %PROTO_PATH%/Scene.proto