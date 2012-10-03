cmake_minimum_required(VERSION 2.8)
project(ktftd)

set(CMAKE_C_FLAGS "-std=gnu99 -fvisibility=hidden -pthread -Wall")
set(CMAKE_CXX_FLAGS "-std=gnu++0x -fvisibility=hidden -pthread -Wall")
set(CMAKE_MODULE_LINKER_FLAGS "-fvisibility=hidden -pthread -Wall")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(KTFTD_SOURCE_FILES
				src/ktftd.cpp
				src/es2_gfx.cpp
				src/sdlaudio.cpp
				)

include_directories("${PROJECT_SOURCE_DIR}/inc")

add_executable(ktftd ${KTFTD_SOURCE_FILES})


add_subdirectory(lib)
add_subdirectory(util)

target_link_libraries(ktftd ktftdutil)
find_package(SDL2 REQUIRED)
target_link_libraries(ktftd ${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})

find_package(GLESV2 REQUIRED)
target_link_libraries(ktftd ${GLESV2_LIBRARIES})
include_directories(${GLESV2_INCLUDE_DIRS})
