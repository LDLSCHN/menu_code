################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/MyEncoder/MyEncoder.c" 

COMPILED_SRCS += \
"code/MyEncoder/MyEncoder.src" 

C_DEPS += \
"./code/MyEncoder/MyEncoder.d" 

OBJS += \
"code/MyEncoder/MyEncoder.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/MyEncoder/MyEncoder.src":"../code/MyEncoder/MyEncoder.c" "code/MyEncoder/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/LDLS/AURIX-v1.10.24-workspace/menu_code/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/MyEncoder/MyEncoder.o":"code/MyEncoder/MyEncoder.src" "code/MyEncoder/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-MyEncoder

clean-code-2f-MyEncoder:
	-$(RM) ./code/MyEncoder/MyEncoder.d ./code/MyEncoder/MyEncoder.o ./code/MyEncoder/MyEncoder.src

.PHONY: clean-code-2f-MyEncoder

