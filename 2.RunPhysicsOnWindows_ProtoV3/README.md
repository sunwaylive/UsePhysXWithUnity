#1.How to run the solution:

1. Open ServerCooperationMode.sln , choose: x64 platform

2. Install OpenGL, freeglut.

3. Run solusion.

#2. Folder Specification
1. Bin/: *.dll are here, runtime neede libraries, including PhysX and OpenGL
2. BuildProto/: protobuf, to transform Scene.proto to C++ codes
3. external/: needed 3rd party libraries
4. IncludeLib/: include files and *.lib files are here, including PhysX and Protobuf
5. ProtoGeneratedScrips/: when you run BuildProto/BuildAll.bat, generated c++ codes will be here
6. Scenes/: Game Scenes exported from Unity Editor
7. ServerCooperationMode/: solution files and source codes

#3. Modify Scene.proto
1. Modify BuidProto/Protos/Scene.proto
2. Run BuidProto/BuildAll.bat, and you will see new files in ProtoGeneratedScrips/