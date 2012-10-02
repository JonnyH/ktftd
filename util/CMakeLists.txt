option (BUILD_KTFTDIMG
				"Build ktftdimg - a tool for loading and converting tftd image
				resources" ON)

option (BUILD_KTFTDVID
				"Build ktftdvid - a tool for loading and converting tftd video
				resources" ON)

option (BUILD_KTFTDSND
				"Build ktftdsnd - a tool for loading and converting tftd audio
				resources" ON)

if (BUILD_KTFTDIMG)
	set (KTFTDIMG_SOURCE_FILES
					img.cpp)
	add_executable(ktftdimg ${KTFTDIMG_SOURCE_FILES})
	target_link_libraries(ktftdimg ktftdutil)
endif (BUILD_KTFTDIMG)

if (BUILD_KTFTDVID)
	set (KTFTDVID_SOURCE_FILES
					flc.cpp)
	add_executable(ktftdvid ${KTFTDVID_SOURCE_FILES})
	target_link_libraries(ktftdvid ktftdutil)
endif (BUILD_KTFTDVID)

if (BUILD_KTFTDSND)
				set (KTFTDSND_SOURCE_FILES
					snd.cpp)
	add_executable(ktftdsnd ${KTFTDSND_SOURCE_FILES})
	target_link_libraries(ktftdsnd ktftdutil)
endif(BUILD_KTFTDSND)
