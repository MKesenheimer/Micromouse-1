################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../micromouse.cmd 

CFG_SRCS += \
../micromouse.cfg 

C_SRCS += \
../main.c \
../system.c 

OBJS += \
./main.obj \
./system.obj 

C_DEPS += \
./main.pp \
./system.pp 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

C_DEPS__QUOTED += \
"main.pp" \
"system.pp" 

OBJS__QUOTED += \
"main.obj" \
"system.obj" 

C_SRCS__QUOTED += \
"../main.c" \
"../system.c" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 


