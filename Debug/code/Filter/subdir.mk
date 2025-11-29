################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/Filter/Filter.c" 

COMPILED_SRCS += \
"code/Filter/Filter.src" 

C_DEPS += \
"./code/Filter/Filter.d" 

OBJS += \
"code/Filter/Filter.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/Filter/Filter.src":"../code/Filter/Filter.c" "code/Filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/LDLS/AURIX-v1.10.24-workspace/menu_code/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/Filter/Filter.o":"code/Filter/Filter.src" "code/Filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Filter

clean-code-2f-Filter:
	-$(RM) ./code/Filter/Filter.d ./code/Filter/Filter.o ./code/Filter/Filter.src

.PHONY: clean-code-2f-Filter

