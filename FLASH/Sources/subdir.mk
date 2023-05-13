################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Blazar_TFTLCD.c" \
"../Sources/KL2x_gpio.c" \
"../Sources/MMA8451Q.c" \
"../Sources/button.c" \
"../Sources/judge.c" \
"../Sources/light.c" \
"../Sources/main.c" \
"../Sources/music.c" \
"../Sources/sa_mtb.c" \
"../Sources/systick.c" \

C_SRCS += \
../Sources/Blazar_TFTLCD.c \
../Sources/KL2x_gpio.c \
../Sources/MMA8451Q.c \
../Sources/button.c \
../Sources/judge.c \
../Sources/light.c \
../Sources/main.c \
../Sources/music.c \
../Sources/sa_mtb.c \
../Sources/systick.c \

OBJS += \
./Sources/Blazar_TFTLCD.o \
./Sources/KL2x_gpio.o \
./Sources/MMA8451Q.o \
./Sources/button.o \
./Sources/judge.o \
./Sources/light.o \
./Sources/main.o \
./Sources/music.o \
./Sources/sa_mtb.o \
./Sources/systick.o \

C_DEPS += \
./Sources/Blazar_TFTLCD.d \
./Sources/KL2x_gpio.d \
./Sources/MMA8451Q.d \
./Sources/button.d \
./Sources/judge.d \
./Sources/light.d \
./Sources/main.d \
./Sources/music.d \
./Sources/sa_mtb.d \
./Sources/systick.d \

OBJS_QUOTED += \
"./Sources/Blazar_TFTLCD.o" \
"./Sources/KL2x_gpio.o" \
"./Sources/MMA8451Q.o" \
"./Sources/button.o" \
"./Sources/judge.o" \
"./Sources/light.o" \
"./Sources/main.o" \
"./Sources/music.o" \
"./Sources/sa_mtb.o" \
"./Sources/systick.o" \

C_DEPS_QUOTED += \
"./Sources/Blazar_TFTLCD.d" \
"./Sources/KL2x_gpio.d" \
"./Sources/MMA8451Q.d" \
"./Sources/button.d" \
"./Sources/judge.d" \
"./Sources/light.d" \
"./Sources/main.d" \
"./Sources/music.d" \
"./Sources/sa_mtb.d" \
"./Sources/systick.d" \

OBJS_OS_FORMAT += \
./Sources/Blazar_TFTLCD.o \
./Sources/KL2x_gpio.o \
./Sources/MMA8451Q.o \
./Sources/button.o \
./Sources/judge.o \
./Sources/light.o \
./Sources/main.o \
./Sources/music.o \
./Sources/sa_mtb.o \
./Sources/systick.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Blazar_TFTLCD.o: ../Sources/Blazar_TFTLCD.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Blazar_TFTLCD.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Blazar_TFTLCD.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/KL2x_gpio.o: ../Sources/KL2x_gpio.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/KL2x_gpio.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/KL2x_gpio.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/MMA8451Q.o: ../Sources/MMA8451Q.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/MMA8451Q.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/MMA8451Q.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/button.o: ../Sources/button.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/button.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/button.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/judge.o: ../Sources/judge.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/judge.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/judge.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/light.o: ../Sources/light.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/light.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/light.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/music.o: ../Sources/music.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/music.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/music.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/systick.o: ../Sources/systick.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/systick.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/systick.o"
	@echo 'Finished building: $<'
	@echo ' '


