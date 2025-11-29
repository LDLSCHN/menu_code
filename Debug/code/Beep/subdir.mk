################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/Beep/Beep.c" 

COMPILED_SRCS += \
"code/Beep/Beep.src" 

C_DEPS += \
"./code/Beep/Beep.d" 

OBJS += \
"code/Beep/Beep.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/Beep/Beep.src":"../code/Beep/Beep.c" "code/Beep/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/LDLS/AURIX-v1.10.24-workspace/menu_code/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/Beep/Beep.o":"code/Beep/Beep.src" "code/Beep/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Beep

clean-code-2f-Beep:
	-$(RM) ./code/Beep/Beep.d ./code/Beep/Beep.o ./code/Beep/Beep.src

.PHONY: clean-code-2f-Beep

