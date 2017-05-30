find_library(ZIP "libzip.a" REQUIRED)
add_library(ziplib STATIC IMPORTED )
set_property(TARGET ziplib PROPERTY IMPORTED_LOCATION ${ZIP})
