cmake_minimum_required (VERSION 3.2)
project (pebble_app)

set(CMAKE_C_FLAGS "-std=c11 -mthumb -ffunction-sections -g -fno-diagnostics-show-caret -D_REENT_SMALL=1 -Wall -Wextra -Werror -Wpointer-arith -Wno-unused-parameter -Wno-missing-field-initializers -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=unused-but-set-variable -Wno-packed-bitfield-compat -mcpu=cortex-m3 -Os -Werror=return-type")

file(GLOB_RECURSE src "src/*")
file(GLOB_RECURSE build "build/chalk/*.c")

set(SOURCES
  ${src}
  ${build}
  )

set(INCLUDES
  /usr/local/Cellar/pebble-sdk/3.6-dp2/Pebble/chalk/include
)

add_definitions(-DPEBBLE)

include_directories(${INCLUDES})

add_executable(app ${SOURCES})
