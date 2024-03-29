find_package(PkgConfig)

pkg_check_modules(PC_GLESV2 REQUIRED sdl2)

find_path(GLESV2_INCLUDE_DIR SDL.h HINTS ${PC_GLESV2_INCLUDE_DIRS}
				${PC_GLESV2_INCLUDEDIR} PATH_SUFFIXES GLESV2)

find_library(GLESV2_LIBRARY NAMES GLESv2 libGLESv2 HINTS ${PC_GLESV2_LIBDIR}
				${PC_GLESV2_LIBRARY_DIRS} )

set(GLESV2_LIBRARIES ${GLESV2_LIBRARY})
set(GLESV2_INCLUDE_DIRS ${GLESV2_INCLUDE_DIR})

find_package_handle_standard_args(GLESV2 DEFAULT_MSG GLESV2_LIBRARY GLESV2_INCLUDE_DIR)
mark_as_advanced(GLESV2_INCLUDE_DIR GLESV2_LIBRARY)
