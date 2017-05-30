include(cmakemodules/script_libz.cmake REQUIRED)
include(cmakemodules/script_libzip.cmake REQUIRED)
include(cmakemodules/script_libqt.cmake REQUIRED)
include(cmakemodules/script_libboost.cmake REQUIRED)
include(cmakemodules/script_libexpat.cmake REQUIRED)
include(cmakemodules/script_libxls.cmake REQUIRED)

find_package( Mlpack REQUIRED )
INCLUDE_DIRECTORIES(${Mlpack_INCLUDE_DIR} )

find_package( Armadillo REQUIRED )
INCLUDE_DIRECTORIES(${Armadillo_INCLUDE_DIR} )



