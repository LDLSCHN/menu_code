################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/PID/PID.c" 

COMPILED_SRCS += \
"code/PID/PID.src" 

C_DEPS += \
"./code/PID/PID.d" 

OBJS += \
"code/PID/PID.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/PID/PID.src":"../code/PID/PID.c" "code/PID/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/LDLS/AURIX-v1.10.24-workspace/menu_code/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/PID/PID.o":"code/PID/PID.src" "code/PID/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-PID

clean-code-2f-PID:
	-$(RM) ./code/PID/PID.d ./code/PID/PID.o ./code/PID/PID.src

.PHONY: clean-code-2f-PID

