cmake_minimum_required(VERSION 3.5)
project(Diploma)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extensions/)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(GUI_APP_NAME ${PROJECT_NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED PATHS ${QT_DIR})
find_package( Armadillo REQUIRED )
find_package( Mlpack REQUIRED )

set(MAIN_FILES
	main.cpp
	mainwindow.cpp
	mainworkplace.cpp
	regression.cpp
	table.cpp
	mainwindow.ui
	 )


# Add executables files:
add_executable(${GUI_APP_NAME} ${MAIN_FILES})



#This is the command to connect libraries to a target:
target_link_libraries(${GUI_APP_NAME}
Qt5::Core
Qt5::Widgets
${Mlpack_LIBRARIES}
${Armadillo_LIBRARIES}
)


