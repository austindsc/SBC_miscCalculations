cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(ATOWN_NEST_TEST VERSION 1.0 LANGUAGES CXX)

## getting GCEM (NEST doesn't load it itself?)
set(gcem_DIR $ENV{NESTNEW_INSTALL}/lib/cmake/gcem/)
# message("gcem_DIR:    " ${gcem_DIR})
find_package(gcem REQUIRED)

# trying to find nestcore 
list(APPEND CMAKE_PREFIX_PATH $ENV{NESTNEW_INSTALL}/lib/)
find_library(ncpath NESTCore HINTS $ENV{NESTNEW_INSTALL}/lib/)

set(NEST_DIR $ENV{NESTNEW_INSTALL}/lib/)
find_package(NEST REQUIRED)

# some of these are probably unecessary but oh well
set(MY_NEST_INCLUDE $ENV{NESTNEW_INSTALL}/include/)
set(MY_NEST_INCLUDE1 $ENV{NESTNEW_INSTALL}/include/NEST)
set(MY_NEST_INCLUDE2 $ENV{NESTNEW_INSTALL}/include/Detectors)
set(MY_NEST_INCLUDE3 $ENV{NESTNEW_INSTALL}/include/gcem_incl)
include_directories(${MY_NEST_INCLUDE})
include_directories(${MY_NEST_INCLUDE1})
include_directories(${MY_NEST_INCLUDE2})
include_directories(${MY_NEST_INCLUDE3})

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

#define source file variable 'SOURCES'
file(GLOB SOURCES   ${PROJECT_SOURCE_DIR}/*.cc
                    )
                    
add_executable(sbctt sbctt.cpp ${CMAKE_CXX_FLAGS})
target_compile_features(sbctt PUBLIC cxx_std_20)
target_link_libraries(sbctt PUBLIC ${ncpath} gcem ${ROOT_LIBRARIES})