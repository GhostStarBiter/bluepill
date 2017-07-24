cmake_minimum_required(VERSION 3.7)
project(bluepill C ASM)

add_definitions(-DSTM32F103xB)

file(GLOB_RECURSE USER_SOURCES "Src/*.c")
file(GLOB_RECURSE HAL_SOURCES "Drivers/STM32F1xx_HAL_Driver/Src/*.c")

include_directories(Inc)
include_directories(Drivers/STM32F1xx_HAL_Driver/Inc)
include_directories(Drivers/CMSIS/Include)
include_directories(Drivers/CMSIS/Device/ST/STM32F1xx/Include)

add_library(CMSIS
        /home/mrgreen/STM32Cube/Repository/STM32Cube_FW_F1_V1.4.0/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
        startup/startup_stm32f103xb.s)

add_executable(${PROJECT_NAME}.elf ${USER_SOURCES} ${HAL_SOURCES} ${LINKER_SCRIPT})

target_link_libraries(${PROJECT_NAME}.elf CMSIS)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.map")
set(HEX_FILE ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")
