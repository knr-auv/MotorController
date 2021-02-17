set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "arm")

set(CMAKE_MAKE_PROGRAM make)
set(tools "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2020-q4-major")
set(suffix ".exe")

set(prefix "arm-none-eabi")
set(compiler_flags "-DSTM32F411xE -Wall -fdata-sections -ffunction-sections -Os -fno-math-errno")
set(linker_script "${CMAKE_SOURCE_DIR}/platform/STM32F411RE_FLASH.ld")
set(mcu "-mcpu=cortex-m4 -mthumb")
set(linker_flags "-specs=nano.specs -T${linker_script} -lc -lm -lnosys -Wl,--gc-sections")

set(CMAKE_C_COMPILER "${tools}/bin/${prefix}-gcc${suffix}")
set(CMAKE_C_COMPILER_AR "${tools}/bin/${prefix}-ar${suffix}")
set(CMAKE_ASM_COMPILER "${tools}/bin/${prefix}-gcc${suffix}")
set(CMAKE_LINKER "${tools}/bin/${prefix}-ld${suffix}")
set(CMAKE_C_COMPILER_WORKS 1)
set(OBJCOPY "${tools}/bin/${prefix}-objcopy${suffix}")
set(SIZE "${tools}/bin/${prefix}-size${suffix}")


set(CMAKE_C_FLAGS "${mcu} ${compiler_flags}")
set(CMAKE_ASM_FLAGS "${mcu} ${compiler_flags}")
set(CMAKE_EXE_LINKER_FLAGS "${mcu} ${linker_flags}") 

SET(ASM_OPTIONS "-x assembler-with-cpp")