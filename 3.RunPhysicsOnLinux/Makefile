PRGNAME = libphysx_wrapper.a
OUTFILE = $(PRGNAME)

#IGNORE_DIRS := test

#LIBFILE = $(OUTFILE)
EXEFILE = $(OUTFILE)

include ../Makefile.macros

ALL_DIRS = $(shell ls -l | grep ^d | awk '{print $$NF}')
SUB_DIRS := $(filter-out ${IGNORE_DIRS}, $(ALL_DIRS))

ALL_CPP_FILES += $(wildcard *.cpp )
ALL_CPP_FILES += $(foreach sub_dir, ${SUB_DIRS}, $(wildcard ${sub_dir}/*.cpp))

CPP_FILE := ${ALL_CPP_FILES}

#INC_OWN=-I${KL_SVR_PROJ}/thirdparty/PhysXSDK/Include/
#LIB_OWN=-L${KL_SVR_PROJ}/thirdparty/PhysXSDK/Bin/linux64 -lPhysX3_x64 -lPhysX3CommonDEBUG_x64
LIB_OWN+=-L${KL_SVR_PROJ}/thirdparty/PhysXSDK/Lib/linux64
LIB_OWN+=-lPhysX3_x64
#LIB_OWN += -lPvdRuntime
LIB_OWN +=-lSimulationController \
-lSceneQuery \
-lLowLevelCloth \
-lLowLevel \
-lPhysX3Extensions \
-lPhysXProfileSDK \
-lPhysX3Common_x64 \
-lPxTask \
-lrt


include ../Makefile.rule

echo:
	@echo ${CPP_FILE}


