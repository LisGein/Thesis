cmake_minimum_required(VERSION 3.5)
project(Diploma)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extensions/)
set(CMAKE_CXX_STANDARD 14)
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

INCLUDE_DIRECTORIES(treeWidget)
INCLUDE_DIRECTORIES(experimentWidget)
set(CPP_FILES
	common/qt.cpp
	main.cpp

	mainWindow/mainWindow.cpp
	mainWindow/viewController.cpp

	featureWidget/featureGraphicsScene.cpp
	featureWidget/featureGraphicsItem.cpp
	featureWidget/featureHeaderGraphicsItem.cpp

	datasetWidget/datasetWidget.cpp
	datasetWidget/datasetView.cpp
	datasetWidget/datasetModel.cpp

	documentTree/regression.cpp
	documentTree/linearRegressionModel.cpp
	documentTree/document.cpp
	documentTree/iNode.cpp
	documentTree/experiment.cpp
	documentTree/featureModel.cpp
	documentTree/dataset.cpp

	treeWidget/treeModel.cpp
	treeWidget/treeView.cpp

	dataPlotWidget/dataPlotWidget.cpp

	experimentWidget/experimentWidget.cpp
	experimentWidget/experimentTreeModel.cpp
	experimentWidget/experimentTable.cpp
	 )

 set(FILES_TO_MOC
	 mainWindow/mainWindow.h
	 mainWindow/viewController.h

	 featureWidget/featureGraphicsScene.h

	 datasetWidget/datasetView.h
	 dataPlotWidget/dataPlotWidget.h

	 treeWidget/treeView.h
	 treeWidget/treeModel.h

	 experimentWidget/experimentWidget.h
	 experimentWidget/experimentTreeModel.h
	 experimentWidget/experimentTable.h
	  )

 set(FILES_TO_UIC
	 dataPlotWidget/dataPlotWidget.ui
	 mainWindow/mainWindow.ui
	 datasetWidget/datasetWidget.ui
	 experimentWidget/experimentWidget.ui
	 )


qt5_wrap_cpp(moc_cpps ${FILES_TO_MOC})
qt5_wrap_ui(uic_cpps ${FILES_TO_UIC})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
# Add executables files:
add_executable(${GUI_APP_NAME} ${CPP_FILES} ${moc_cpps} ${uic_cpps})



#This is the command to connect libraries to a target:
target_link_libraries(${GUI_APP_NAME}
Qt5::Core
Qt5::Widgets
Qt5::Charts
${Mlpack_LIBRARIES}
${Armadillo_LIBRARIES}
${Boost_LIBRARIES}
)

