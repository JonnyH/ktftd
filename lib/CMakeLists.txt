cmake_minimum_required(VERSION 2.8)

set(KTFTDUTIL_SOURCE_FILES
				bdy.cpp
				lbm.cpp
				palette_image.cpp
				scr.cpp
				spk.cpp
)

add_library(ktftdutil ${KTFTDUTIL_SOURCE_FILES})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
include_directories("${PROJECT_SOURCE_DIR}/inc")
target_link_libraries(ktftdutil ${PNG_LIBRARY})
				
