cmake_minimum_required(VERSION 3.5)
project(Diploma)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extensions/)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(GUI_APP_NAME ${PROJECT_NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Widgets Charts REQUIRED PATHS ${QT_DIR})
find_package( Mlpack REQUIRED )
INCLUDE_DIRECTORIES(${Mlpack_INCLUDE_DIR} )

find_package( Armadillo REQUIRED )
INCLUDE_DIRECTORIES(${Armadillo_INCLUDE_DIR} )

FIND_PACKAGE( Boost REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(CPP_FILES
	common/qt.cpp
	main.cpp
	mainWindow.cpp
	mainWidget.cpp
	featureWidget/featureGraphicsScene.cpp
	featureWidget/featureGraphicsItem.cpp
	featureWidget/featureHeaderGraphicsItem.cpp
	featureModel.cpp
	regression.cpp
	datasetWidget/datasetView.cpp
	datasetWidget/datasetModel.cpp
	document.cpp
	dataset.cpp
	 )

 set(FILES_TO_MOC
	 mainWindow.h
	 mainWidget.h
	 featureWidget/featureGraphicsScene.h
#	 featureWidget/featureGraphicsItem.h
#	 featureWidget/featureHeaderGraphicsItem.h
	 datasetWidget/datasetView.h
	  )


qt5_wrap_cpp(moc_cpps ${FILES_TO_MOC})

# Add executables files:
add_executable(${GUI_APP_NAME} ${CPP_FILES} ${moc_cpps})



#This is the command to connect libraries to a target:
target_link_libraries(${GUI_APP_NAME}
Qt5::Core
Qt5::Widgets
Qt5::Charts
${Mlpack_LIBRARIES}
${Armadillo_LIBRARIES}
${Boost_LIBRARIES}
)


