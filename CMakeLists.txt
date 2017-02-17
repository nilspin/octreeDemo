project(octreeDemo)
cmake_minimum_required(VERSION 3.5)

# Useful CMake options for Qt projects
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-std=c++0x")
cmake_policy(SET CMP0004 OLD)

#--------------------------------------
#Find GLM
#find_package(glm REQUIRED)
#include_directories(${GLM_INCLUDE_DIRS})
#link_directories(${GLM_LIBRARY_DIRS})
#add_definitions(${GLM_DEFINITIONS})

#if(NOT GLM_FOUND)
#        message(Error "GLM not found")
#endif(NOT GLM_FOUND)
#---------------------------------------

# Additional packages/libraries
INCLUDE(FindPkgConfig)
#GLM
INCLUDE_DIRECTORIES(
    libs/glm-0.9.7
)


set(INCLUDE_DIRS ${INCLUDE_DIRS} ) #${GLM_INCLUDE_DIRS}
set(LIBS ${LIBS} ) #${GLM_DEFINITIONS}

#specify include directory
include_directories(INCLUDE_DIRS ${INCLUDE_DIRS} include)

set( project_sources
   octreeMain.cpp
   octree.h
)

add_executable(${PROJECT_NAME} octreeMain.cpp octree.h)

target_link_libraries(${PROJECT_NAME} ${LIBS})
