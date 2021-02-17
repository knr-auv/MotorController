cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_C_STANDARD 11)


#flags for FPU 
add_compile_options("-mfloat-abi=hard")
add_compile_options("-mfpu=fpv4-sp-d16")
add_link_options("-mfloat-abi=hard")
add_link_options("-mfpu=fpv4-sp-d16")

add_definitions(-D__FPU_PRESENT )


set(TARGET "main")

set(CMAKE_BUILD_DIRECTORY bin)
set(TARGET_ELF "${TARGET}.elf")
set(TARGET_HEX "${TARGET}.hex")
set(TARGET_BIN "${TARGET}.bin")

project(STM-Controller C ASM)



add_executable(${TARGET_ELF}
    src/setup/setup.c

    src/test/test.c
    src/core/Scheduler.c
    src/core/tasks.c

    src/JetsonComunication/JetsonComunication.c
    src/JetsonComunication/uart_tx_rx.c

    src/sensors/attitude.c
    src/sensors/depth.c
    src/sensors/gyro.c
    
    src/PIDs/Controls.c
    src/PIDs/PIDController.c
    src/PIDs/PIDMath.c

    src/input/input_jetson.c

    src/math/Quaternions.c

    drivers/Std_Periph/src/stm32f4xx_dma.c
    drivers/Std_Periph/src/stm32f4xx_usart.c
    drivers/Std_Periph/src/stm32f4xx_rcc.c
    drivers/Std_Periph/src/misc.c
    drivers/Std_Periph/src/stm32f4xx_tim.c
    drivers/Std_Periph/src/stm32f4xx_gpio.c
    platform/startup_stm32f40_41xxx.s
    platform/system_stm32f4xx.c
    drivers/src/time.c
    src/main.c)

target_include_directories(${TARGET_ELF} PUBLIC
    src
    drivers/inc
    drivers/Std_Periph/inc
    platform
    platform/CMSIS/Include)
    
  #arm_math is slower than math.h. It has some interesting features like PID - worth checking
  #target_link_libraries(${TARGET_ELF} "C:/Users/Kajetan/source/repos/STM-controller/platform/arm_cortexM4lf_math.lib")  

  target_link_libraries(${TARGET_ELF} m c) 


add_custom_target(${TARGET_HEX} ALL COMMAND ${OBJCOPY} -O ihex ${TARGET_ELF} ${TARGET_HEX})
add_dependencies(${TARGET_HEX} ${TARGET_ELF})
add_custom_target(${TARGET_BIN} ALL COMMAND ${OBJCOPY} -O binary -S ${TARGET_ELF} ${TARGET_BIN})
add_dependencies(${TARGET_BIN} ${TARGET_ELF})
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD COMMAND ${SIZE} ${TARGET_ELF})
add_custom_target(program "openocd" "-f board/st_nucleo_f4.cfg -c \"program ${TARGET_ELF} reset exit\"")
