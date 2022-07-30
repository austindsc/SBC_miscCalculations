cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(ATOWN_NEST_TEST VERSION 1.0 LANGUAGES CXX)

# set(gcem_DIR "/Users/austindestcroix/packages/nest2install/lib/cmake/gcem/")
# message("gcem_DIR:    " ${gcem_DIR})

set(NEST_DIR "/Users/austindestcroix/packages/nest2install/")
find_package(NEST REQUIRED)
include_directories(${NEST_INCLUDE_DIRS})
message("NEST_INCLUDE_DIRS:    " ${NEST_INCLUDE_DIRS})

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

add_executable(test main1.cpp)
target_link_libraries(test PUBLIC NEST ${ROOT_LIBRARIES})