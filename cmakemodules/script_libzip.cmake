find_library(ZIP "libzip.so" REQUIRED)
add_library(ziplib SHARED IMPORTED )
set_property(TARGET ziplib PROPERTY IMPORTED_LOCATION ${ZIP})
