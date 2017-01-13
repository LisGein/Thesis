cmake_minimum_required(VERSION 3.1)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)

# Basic information about project
project(BurnCalc)

set(CMAKE_AUTOMOC ON)


set(SOURCE_FILES
    src/element.cpp
    src/chemicalSubstance.cpp
    src/mixture.cpp
    src/reaction.cpp
    src/runtimeerror.cpp
)

set(GUI_SOURCE_FILES
    src/main.cpp
    src/mainwindow.cpp
    src/gui/guisubstance.cpp
    src/gui/guiempericalformula.cpp
)

find_package(Qt5Widgets REQUIRED)

INCLUDE_DIRECTORIES(YAML_CPP_INCLUDE_DIR)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.58.0 COMPONENTS system REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})


find_package(yaml-cpp REQUIRED)
INCLUDE_DIRECTORIES(YAML_CPP_INCLUDE_DIR)


QT5_WRAP_UI(UIS_HDRS
    src/mainwindow.ui
)

set(GUI_APP_NAME ${PROJECT_NAME})

add_executable(${GUI_APP_NAME}
    ${SOURCE_FILES}
    ${Boost_LIBRARIES}
    ${GUI_SOURCE_FILES}
    ${UIS_HDRS}
)

qt5_use_modules(${GUI_APP_NAME} Widgets)
target_link_libraries(${GUI_APP_NAME}
   ${YAML_CPP_LIBRARIES}
   )


# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set(TESTS_FILES
   src/tests/tests.cpp
   src/tests/chemicalSubstanceTests.cpp
   )

add_executable(runUnitTests
   ${TESTS_FILES}
   ${SOURCE_FILES}
)

target_link_libraries(runUnitTests
   ${GTEST_LIBRARIES}
   ${Boost_LIBRARIES}
   ${YAML_CPP_LIBRARIES}
   pthread
   )
