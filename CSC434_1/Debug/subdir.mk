################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
F77_SRCS += \
../main.f77 

OBJS += \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.f77
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Fortran Compiler'
	gfortran -funderscoring -O0 -g -Wall -c -fmessage-length=0 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: ../main.f77


