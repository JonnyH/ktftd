cmake_minimum_required(VERSION 2.8)
project(tftdimg)

set(CMAKE_C_FLAGS "-std=gnu99 -fvisibility=hidden -pthread -Wall")
set(CMAKE_CXX_FLAGS "-std=gnu++0x -fvisibility=hidden -pthread -Wall")
set(CMAKE_MODULE_LINKER_FLAGS "-fvisibility=hidden -pthread -Wall")

set(TFTPIMG_SOURCE_FILES
				img.cpp
				lbm.cpp
				spk.cpp
				scr.cpp
				bdy.cpp
				palette_image.cpp
				)


add_executable(tftdimg ${TFTPIMG_SOURCE_FILES})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(tftdimg ${PNG_LIBRARY})

set(TFTDFLC_SOURCE_FILES
				flc.cpp
				)

add_executable(tftdflc ${TFTDFLC_SOURCE_FILES})


