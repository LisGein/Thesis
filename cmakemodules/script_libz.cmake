find_library(Z_LIB "libz.so" REQUIRED)
add_library(zlib SHARED IMPORTED )
set_property(TARGET zlib PROPERTY IMPORTED_LOCATION ${Z_LIB})
