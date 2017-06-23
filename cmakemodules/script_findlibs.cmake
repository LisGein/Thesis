include(cmakemodules/script_libz.cmake REQUIRED)
include(cmakemodules/script_libzip.cmake REQUIRED)
include(cmakemodules/script_libqt.cmake REQUIRED)
include(cmakemodules/script_libboost.cmake REQUIRED)
include(cmakemodules/script_libexpat.cmake REQUIRED)
include(cmakemodules/script_libxls.cmake REQUIRED)

if (WIN32)

	find_library(BLAS_LIBRARY 
	"blas.lib"
	PATH "$ENV{ProgramFiles}/CLAPACK/lib" )
	
	find_library(LAPACK_LIBRARY 
	"lapack.lib"
	PATH "$ENV{ProgramFiles}/CLAPACK/lib")

	find_library(LIBF2_LIBRARY 
	"libf2c.lib"
	PATH "$ENV{ProgramFiles}/CLAPACK/lib")

	find_library(ARMADILLO_LIBRARY
	NAMES armadillo
	PATHS "$ENV{ProgramFiles}/Armadillo/lib"  "$ENV{ProgramFiles}/Armadillo/lib64" "$ENV{ProgramFiles}/Armadillo"
	)
	

	set(ARMADILLO_INCLUDE_DIR "C:/Program Files/Armadillo/include")
	message(${ARMADILLO_INCLUDE_DIR})
	include_directories( ${ARMADILLO_INCLUDE_DIR} )
	if(ARMADILLO_INCLUDE_DIR)
		set(ARMADILLO_VERSION_MAJOR 0)
		set(ARMADILLO_VERSION_MINOR 0)
		set(ARMADILLO_VERSION_PATCH 0)
		set(ARMADILLO_VERSION_NAME "EARLY RELEASE")

		  if(EXISTS "${ARMADILLO_INCLUDE_DIR}/armadillo_bits/arma_version.hpp")

			# Read and parse armdillo version header file for version number
			file(STRINGS "${ARMADILLO_INCLUDE_DIR}/armadillo_bits/arma_version.hpp" _armadillo_HEADER_CONTENTS REGEX "#define ARMA_VERSION_[A-Z]+ ")
			string(REGEX REPLACE ".*#define ARMA_VERSION_MAJOR ([0-9]+).*" "\\1" ARMADILLO_VERSION_MAJOR "${_armadillo_HEADER_CONTENTS}")
			string(REGEX REPLACE ".*#define ARMA_VERSION_MINOR ([0-9]+).*" "\\1" ARMADILLO_VERSION_MINOR "${_armadillo_HEADER_CONTENTS}")
			string(REGEX REPLACE ".*#define ARMA_VERSION_PATCH ([0-9]+).*" "\\1" ARMADILLO_VERSION_PATCH "${_armadillo_HEADER_CONTENTS}")

			# WARNING: The number of spaces before the version name is not one.
			string(REGEX REPLACE ".*#define ARMA_VERSION_NAME +\"([0-9a-zA-Z _-]+)\".*" "\\1" ARMADILLO_VERSION_NAME "${_armadillo_HEADER_CONTENTS}")

			unset(_armadillo_HEADER_CONTENTS)
		  endif()

		  set(ARMADILLO_VERSION_STRING "${ARMADILLO_VERSION_MAJOR}.${ARMADILLO_VERSION_MINOR}.${ARMADILLO_VERSION_PATCH}")
	endif(ARMADILLO_INCLUDE_DIR)

	if (ARMADILLO_FOUND)
		set(ARMADILLO_INCLUDE_DIRS ${ARMADILLO_INCLUDE_DIR})
		set(ARMADILLO_LIBRARIES ${ARMADILLO_LIBRARY})
	endif ()

	# Hide internal variables
	mark_as_advanced(
	ARMADILLO_INCLUDE_DIR
	ARMADILLO_LIBRARY
	)
else()
	find_package( Armadillo REQUIRED)
endif()

INCLUDE_DIRECTORIES(${Armadillo_INCLUDE_DIR} )



