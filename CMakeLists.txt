cmake_minimum_required(VERSION 3.2)
project(Diploma)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extensions/)
set(CMAKE_CXX_STANDARD 14)
set(GUI_APP_NAME ${PROJECT_NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(cmakemodules/script_findlibs.cmake REQUIRED)

include_directories(treeWidget)
include_directories(experimentWidget)


set(CPP_FILES
	common/qt.cpp
	main.cpp

	mainWindow/mainWindow.cpp
	mainWindow/viewController.cpp
	mainWindow/plotController.cpp

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
	documentTree/datasetColumnsView.cpp

	treeWidget/treeModel.cpp
	treeWidget/treeView.cpp

	dataPlotWidget/dataPlotWidget.cpp
	dataPlotWidget/data3DPlotWidget.cpp
	dataPlotWidget/abstractDataPlot.cpp

	experimentWidget/experimentWidget.cpp
	experimentWidget/datasetAnalysisTable.cpp
	experimentWidget/featureFilterModel.cpp
	)

set(FILES_TO_MOC
	mainWindow/mainWindow.h
	mainWindow/viewController.h
	mainWindow/plotController.h

	featureWidget/featureGraphicsScene.h

	datasetWidget/datasetView.h
	datasetWidget/datasetWidget.h
	datasetWidget/datasetModel.h


	dataPlotWidget/dataPlotWidget.h
	dataPlotWidget/abstractDataPlot.h
	dataPlotWidget/data3DPlotWidget.h

	treeWidget/treeView.h
	treeWidget/treeModel.h

	experimentWidget/experimentWidget.h
	experimentWidget/datasetAnalysisTable.h
	)

set(FILES_TO_UIC
	dataPlotWidget/dataPlotWidget.ui
	mainWindow/mainWindow.ui
	experimentWidget/experimentWidget.ui
	datasetWidget/datasetWidget.ui
	)


qt5_wrap_cpp(moc_cpps ${FILES_TO_MOC})
qt5_wrap_ui(uic_cpps ${FILES_TO_UIC})

set(QON_TS_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/languages/${PROJECT_NAME}_ru.ts
	)
find_package(Qt5LinguistTools REQUIRED)

#qt5_create_translation(QON_QM_FILES ${CPP_FILES} ${QON_TS_FILES} OPTIONS -no-recursive)
qt5_add_translation(QON_QM_FILES ${QON_TS_FILES})

add_custom_target(translations DEPENDS ${QON_QM_FILES})
add_custom_target(tm_files DEPENDS ${QON_QM_FILES})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
# Add executables files:
add_executable(${GUI_APP_NAME} ${CPP_FILES} ${moc_cpps} ${uic_cpps} ${QON_QM_FILES} )


IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)


#This is the command to connect libraries to a target:

set(OLD_GCC_LIBS "")
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)
	set(OLD_GCC_LIBS "m stdc++")
else()
	add_definitions(-DHAS_MAKE_UNIQUE)
endif()

target_link_libraries(${GUI_APP_NAME}
	xls
	expat
	ziplib
	zlib
	Qt5::Core
	Qt5::Widgets
	Qt5::Charts
	Qt5::DataVisualization
	${Mlpack_LIBRARIES}
	${Armadillo_LIBRARIES}
	${Boost_LIBRARIES}
	${OLD_GCC_LIBS}
	)


