################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/menu/menu.c" 

COMPILED_SRCS += \
"code/menu/menu.src" 

C_DEPS += \
"./code/menu/menu.d" 

OBJS += \
"code/menu/menu.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/menu/menu.src":"../code/menu/menu.c" "code/menu/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/LDLS/AURIX-v1.10.24-workspace/menu_code/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/menu/menu.o":"code/menu/menu.src" "code/menu/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-menu

clean-code-2f-menu:
	-$(RM) ./code/menu/menu.d ./code/menu/menu.o ./code/menu/menu.src

.PHONY: clean-code-2f-menu

