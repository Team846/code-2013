# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))
WRVX_COMPBASE := $(subst \,/,$(WRVX_COMPBASE))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC603gnu
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/WindRiver/workspace
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/LRT13



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC603
TOOL_FAMILY = gnu
TOOL = gnu
TOOL_PATH = 
CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I$(WIND_BASE)/target/h/wrn/coreip -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h 

IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 

IDE_DEFINES = 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
DEBUGFLAGS_C-Compiler = -g
else
DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
endif


#Project Targets
PROJECT_TARGETS = LRT13/$(MODE_DIR)/LRT13.out \
	 LRT13_partialImage/$(MODE_DIR)/LRT13_partialImage.o


#Rules

# LRT13
ifeq ($(DEBUG_MODE),1)
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
else
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
LRT13/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
endif
LRT13/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h/wrn/coreip -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h 
LRT13/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
LRT13/$(MODE_DIR)/% : IDE_DEFINES = 
LRT13/$(MODE_DIR)/% : PROJECT_TYPE = DKM
LRT13/$(MODE_DIR)/% : DEFINES = 
LRT13/$(MODE_DIR)/% : EXPAND_DBG = 0
LRT13/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
LRT13/$(MODE_DIR)/% : CPU = PPC603
LRT13/$(MODE_DIR)/% : TOOL_FAMILY = gnu
LRT13/$(MODE_DIR)/% : TOOL = gnu
LRT13/$(MODE_DIR)/% : TOOL_PATH = 
LRT13/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
LRT13/$(MODE_DIR)/% : LIBPATH = 
LRT13/$(MODE_DIR)/% : LIBS = 
LRT13/$(MODE_DIR)/% : OBJ_DIR := LRT13/$(MODE_DIR)

OBJECTS_LRT13 = LRT13_partialImage/$(MODE_DIR)/LRT13_partialImage.o

LRT13/$(MODE_DIR)/LRT13.out : $(OBJECTS_LRT13)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmppc $(OBJECTS_LRT13) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_LRT13) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

LRT13/$(MODE_DIR)/LRT13_compile_file : $(FILE) ;

_clean :: LRT13/$(MODE_DIR)/LRT13_clean

LRT13/$(MODE_DIR)/LRT13_clean : 
	$(TRACE_FLAG)if [ -d "LRT13" ]; then cd "LRT13"; rm -rf $(MODE_DIR); fi


# LRT13_partialImage
ifeq ($(DEBUG_MODE),1)
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
else
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
LRT13_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
endif
LRT13_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h/wrn/coreip -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h 
LRT13_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
LRT13_partialImage/$(MODE_DIR)/% : IDE_DEFINES = 
LRT13_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
LRT13_partialImage/$(MODE_DIR)/% : DEFINES = 
LRT13_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
LRT13_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
LRT13_partialImage/$(MODE_DIR)/% : CPU = PPC603
LRT13_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
LRT13_partialImage/$(MODE_DIR)/% : TOOL = gnu
LRT13_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
LRT13_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
LRT13_partialImage/$(MODE_DIR)/% : LIBPATH = 
LRT13_partialImage/$(MODE_DIR)/% : LIBS = 
LRT13_partialImage/$(MODE_DIR)/% : OBJ_DIR := LRT13_partialImage/$(MODE_DIR)

LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ClimberData.o : $(PRJ_ROOT_DIR)/ComponentData/ClimberData.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/CollectorData.o : $(PRJ_ROOT_DIR)/ComponentData/CollectorData.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ComponentData.o : $(PRJ_ROOT_DIR)/ComponentData/ComponentData.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ConfigLoaderData.o : $(PRJ_ROOT_DIR)/ComponentData/ConfigLoaderData.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/DrivetrainData.o : $(PRJ_ROOT_DIR)/ComponentData/DrivetrainData.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/RobotData.o : $(PRJ_ROOT_DIR)/ComponentData/RobotData.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ShooterData.o : $(PRJ_ROOT_DIR)/ComponentData/ShooterData.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/robotdata.pb.o : $(PRJ_ROOT_DIR)/ComponentData/robotdata.pb.cc $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Climber.o : $(PRJ_ROOT_DIR)/Components/Climber.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Collector.o : $(PRJ_ROOT_DIR)/Components/Collector.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Component.o : $(PRJ_ROOT_DIR)/Components/Component.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ComponentManager.o : $(PRJ_ROOT_DIR)/Components/ComponentManager.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ComponentSystemUnitTest.o : $(PRJ_ROOT_DIR)/Components/ComponentSystemUnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ConfigLoader.o : $(PRJ_ROOT_DIR)/Components/ConfigLoader.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Drivetrain.o : $(PRJ_ROOT_DIR)/Components/Drivetrain.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Shooter.o : $(PRJ_ROOT_DIR)/Components/Shooter.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Config/ConfigManager.o : $(PRJ_ROOT_DIR)/Config/ConfigManager.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Config/Configurable.o : $(PRJ_ROOT_DIR)/Config/Configurable.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/AsyncCANJaguar.o : $(PRJ_ROOT_DIR)/Jaguar/AsyncCANJaguar.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/CANTester.o : $(PRJ_ROOT_DIR)/Jaguar/CANTester.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/Esc.o : $(PRJ_ROOT_DIR)/Jaguar/Esc.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/LRTRobot13.o : $(PRJ_ROOT_DIR)/LRTRobot13.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/LRTRobotBase.o : $(PRJ_ROOT_DIR)/LRTRobotBase.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Log/LogManager.o : $(PRJ_ROOT_DIR)/Log/LogManager.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Log/Loggable.o : $(PRJ_ROOT_DIR)/Log/Loggable.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetBuffer.o : $(PRJ_ROOT_DIR)/Network/NetBuffer.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetClient.o : $(PRJ_ROOT_DIR)/Network/NetClient.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetConnection.o : $(PRJ_ROOT_DIR)/Network/NetConnection.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetPeer.o : $(PRJ_ROOT_DIR)/Network/NetPeer.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetServer.o : $(PRJ_ROOT_DIR)/Network/NetServer.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/Actions/PauseAction.o : $(PRJ_ROOT_DIR)/Operations/Actions/PauseAction.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/AutoActions.o : $(PRJ_ROOT_DIR)/Operations/AutoActions.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/AutonomousRoutines.o : $(PRJ_ROOT_DIR)/Operations/AutonomousRoutines.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/DebouncedJoystick.o : $(PRJ_ROOT_DIR)/Operations/DebouncedJoystick.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/TeleopInputs.o : $(PRJ_ROOT_DIR)/Operations/TeleopInputs.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Pneumatics/Pneumatics.o : $(PRJ_ROOT_DIR)/Pneumatics/Pneumatics.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Process/AsyncProcess.o : $(PRJ_ROOT_DIR)/Process/AsyncProcess.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Process/SynchronizedProcess.o : $(PRJ_ROOT_DIR)/Process/SynchronizedProcess.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/AutoAim.o : $(PRJ_ROOT_DIR)/Sensors/AutoAim.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/DiscLocator.o : $(PRJ_ROOT_DIR)/Sensors/DiscLocator.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/DriveEncoders.o : $(PRJ_ROOT_DIR)/Sensors/DriveEncoders.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/LRTEncoder.o : $(PRJ_ROOT_DIR)/Sensors/LRTEncoder.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Servo/LRTServo.o : $(PRJ_ROOT_DIR)/Servo/LRTServo.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/DriveEncodersUnitTest.o : $(PRJ_ROOT_DIR)/UnitTests/DriveEncodersUnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/EncodersUnitTest.o : $(PRJ_ROOT_DIR)/UnitTests/EncodersUnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/GearToothTest.o : $(PRJ_ROOT_DIR)/UnitTests/GearToothTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/InputUnitTest.o : $(PRJ_ROOT_DIR)/UnitTests/InputUnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/LEDUnitTest.o : $(PRJ_ROOT_DIR)/UnitTests/LEDUnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/NetServerUnitTest.o : $(PRJ_ROOT_DIR)/UnitTests/NetServerUnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/NetUnitTest.o : $(PRJ_ROOT_DIR)/UnitTests/NetUnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ProximityTest.o : $(PRJ_ROOT_DIR)/UnitTests/ProximityTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ServoTest.o : $(PRJ_ROOT_DIR)/UnitTests/ServoTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ShooterTest.o : $(PRJ_ROOT_DIR)/UnitTests/ShooterTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/UnitTest.o : $(PRJ_ROOT_DIR)/UnitTests/UnitTest.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/AsyncPrinter.o : $(PRJ_ROOT_DIR)/Utils/AsyncPrinter.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/PID.o : $(PRJ_ROOT_DIR)/Utils/PID.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/PrintInConstructor.o : $(PRJ_ROOT_DIR)/Utils/PrintInConstructor.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/Profiler.o : $(PRJ_ROOT_DIR)/Utils/Profiler.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/bootstrap.o : $(PRJ_ROOT_DIR)/bootstrap.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall -MD -MP -mlongcall -frtti -Wunused -fimplicit-templates -mno-strict-align -fvolatile $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


OBJECTS_LRT13_partialImage = LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ClimberData.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/CollectorData.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ComponentData.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ConfigLoaderData.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/DrivetrainData.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/RobotData.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ShooterData.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/robotdata.pb.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Climber.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Collector.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Component.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ComponentManager.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ComponentSystemUnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ConfigLoader.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Drivetrain.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Shooter.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Config/ConfigManager.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Config/Configurable.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/AsyncCANJaguar.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/CANTester.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/Esc.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/LRTRobot13.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/LRTRobotBase.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Log/LogManager.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Log/Loggable.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetBuffer.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetClient.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetConnection.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetPeer.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetServer.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/Actions/PauseAction.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/AutoActions.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/AutonomousRoutines.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/DebouncedJoystick.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/TeleopInputs.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Pneumatics/Pneumatics.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Process/AsyncProcess.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Process/SynchronizedProcess.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/AutoAim.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/DiscLocator.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/DriveEncoders.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/LRTEncoder.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Servo/LRTServo.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/DriveEncodersUnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/EncodersUnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/GearToothTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/InputUnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/LEDUnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/NetServerUnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/NetUnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ProximityTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ServoTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ShooterTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/UnitTest.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/AsyncPrinter.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/PID.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/PrintInConstructor.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/Profiler.o \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/bootstrap.o

LRT13_partialImage/$(MODE_DIR)/LRT13_partialImage.o : $(OBJECTS_LRT13_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o "$@" $(OBJECTS_LRT13_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

LRT13_partialImage/$(MODE_DIR)/LRT13_partialImage_compile_file : $(FILE) ;

_clean :: LRT13_partialImage/$(MODE_DIR)/LRT13_partialImage_clean

LRT13_partialImage/$(MODE_DIR)/LRT13_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "LRT13_partialImage" ]; then cd "LRT13_partialImage"; rm -rf $(MODE_DIR); fi

force : 

DEP_FILES := LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ClimberData.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/CollectorData.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ComponentData.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ConfigLoaderData.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/DrivetrainData.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/RobotData.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/ShooterData.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/ComponentData/robotdata.pb.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Climber.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Collector.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Component.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ComponentManager.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ComponentSystemUnitTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/ConfigLoader.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Drivetrain.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Components/Shooter.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Config/ConfigManager.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Config/Configurable.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/AsyncCANJaguar.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/CANTester.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Jaguar/Esc.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/LRTRobot13.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/LRTRobotBase.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Log/LogManager.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Log/Loggable.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetBuffer.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetClient.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetConnection.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetPeer.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Network/NetServer.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/Actions/PauseAction.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/AutoActions.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/AutonomousRoutines.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/DebouncedJoystick.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Operations/TeleopInputs.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Pneumatics/Pneumatics.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Process/AsyncProcess.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Process/SynchronizedProcess.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/AutoAim.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/DiscLocator.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/DriveEncoders.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Sensors/LRTEncoder.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Servo/LRTServo.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/DriveEncodersUnitTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/EncodersUnitTest.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/GearToothTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/InputUnitTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/LEDUnitTest.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/NetServerUnitTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/NetUnitTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ProximityTest.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ServoTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/ShooterTest.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/UnitTests/UnitTest.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/AsyncPrinter.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/PID.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/PrintInConstructor.d \
	 LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/Utils/Profiler.d LRT13_partialImage/$(MODE_DIR)/Objects/LRT13/bootstrap.d
-include $(DEP_FILES)



-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

JOBS?=1
ifeq ($(JOBS),1)
main_all : external_build  $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"
else
main_all : external_build 
	@$(MAKE) -f $(MAKEFILE) --jobs $(JOBS) $(MFLAGS) $(PROJECT_TARGETS);\
	echo "make: built targets of `pwd`"
endif 

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
