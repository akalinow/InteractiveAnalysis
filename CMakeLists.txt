cmake_minimum_required (VERSION 2.6)
project (InteractiveAnalysis)
set( CMAKE_VERBOSE_MAKEFILE on )

include_directories ("${PROJECT_SOURCE_DIR}/include")

set (PROJECT_SRC_PATH "src/")

##https://root.cern.ch/how/integrate-root-my-project-cmake
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO GenVector Gui)
include(${ROOT_USE_FILE})

SET( CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})

add_library(guiAnalysisLib SHARED ${PROJECT_SRC_PATH}/EntryDialog.cxx)

add_executable(guiAnalysis bin/guiAnalysis.cxx)

target_link_libraries(guiAnalysis guiAnalysisLib ${ROOT_LIBRARIES})
