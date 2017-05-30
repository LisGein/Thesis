find_library(EXPAT "libexpat.so" REQUIRED)
add_library(expat SHARED IMPORTED )
set_property(TARGET expat PROPERTY IMPORTED_LOCATION ${EXPAT})
