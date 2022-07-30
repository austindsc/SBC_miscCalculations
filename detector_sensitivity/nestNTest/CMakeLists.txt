cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(ATOWN_NEST_TEST VERSION 1.0 LANGUAGES CXX)

# set(gcem_DIR "/Users/austindestcroix/packages/nest2install/lib/cmake/gcem/")
# message("gcem_DIR:    " ${gcem_DIR})

set(gcem_DIR "/Users/austindestcroix/packages/nestNinstall/lib/cmake/gcem/")
find_package(gcem REQUIRED)
# message("gcem_DIR:    " ${gcem_DIR})

# trying to find nestcore 
list(APPEND CMAKE_PREFIX_PATH "/Users/austindestcroix/packages/nestNinstall/lib/")
find_library(NCPATH NESTCore HINTS "/Users/austindestcroix/packages/nestNinstall/lib/")

set(NEST_DIR "/Users/austindestcroix/packages/nestNinstall/lib/")
find_package(NEST REQUIRED)

set(MY_NEST_INCLUDE "/Users/austindestcroix/packages/nestNinstall/include/")
set(MY_NEST_INCLUDE1 "/Users/austindestcroix/packages/nestNinstall/include/NEST")
set(MY_NEST_INCLUDE2 "/Users/austindestcroix/packages/nestNinstall/include/Detectors")
set(MY_NEST_INCLUDE3 "/Users/austindestcroix/packages/nestNinstall/include/gcem_incl")
include_directories(${MY_NEST_INCLUDE})
include_directories(${MY_NEST_INCLUDE1})
include_directories(${MY_NEST_INCLUDE2})
include_directories(${MY_NEST_INCLUDE3})
# include_directories(${NEST_INCLUDE_DIRS})
# message("NEST_INCLUDE_DIRS:    " ${NEST_INCLUDE_DIRS})

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

add_executable(test main1.cpp)
target_link_libraries(test PUBLIC ${NCPATH} gcem ${ROOT_LIBRARIES})