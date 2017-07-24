INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
SET(CMAKE_OBJCOPY arm-none-eabi-objcopy)
SET(CMAKE_C_LINKER_PREFERENCE  arm-none-eabi-ld)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld)
#SET(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=soft -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")
SET(COMMON_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")
SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99 -g")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -T ${LINKER_SCRIPT}")
