include(cmakemodules/script_libz.cmake REQUIRED)
include(cmakemodules/script_libzip.cmake REQUIRED)
include(cmakemodules/script_libqt.cmake REQUIRED)
include(cmakemodules/script_libboost.cmake REQUIRED)
include(cmakemodules/script_libexpat.cmake REQUIRED)
include(cmakemodules/script_libxls.cmake REQUIRED)

if (WIN32)
	find_package( Armadillo PATHS "$ENV{ProgramFiles}/Armadillo/share/Armadillo/CMake" REQUIRED)
else()
	find_package( Armadillo REQUIRED)
endif()

INCLUDE_DIRECTORIES(${Armadillo_INCLUDE_DIR} )



