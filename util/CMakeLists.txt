option (BUILD_KTFTDIMG
				"Build ktftdimg - a tool for loading and converting tftd image
				resources" ON)

if (BUILD_KTFTDIMG)
	set (KTFTDIMG_SOURCE_FILES
					img.cpp)
	add_executable(ktftdimg ${KTFTDIMG_SOURCE_FILES})
	target_link_libraries(ktftdimg ktftdutil)
endif (BUILD_KTFTDIMG)
