if (WIN32)
	find_library(ZIP "zip.lib" PATH "C:\\\\Program Files\\libzip\\lib" REQUIRED)
	add_library(ziplib STATIC IMPORTED )
	set_property(TARGET ziplib PROPERTY IMPORTED_LOCATION ${ZIP})
else()
	find_library(ZIP "libzip.so" REQUIRED)
	add_library(ziplib SHARED IMPORTED )
	set_property(TARGET ziplib PROPERTY IMPORTED_LOCATION ${ZIP})
endif()
