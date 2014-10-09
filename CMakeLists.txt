cmake_minimum_required(VERSION 2.8)

project(cppsproto)
set(LIB_SRC sproto.cpp sprotomessage.cpp sproto.c)
set(CMAKE_C_FLAGS "-Wall -g -O2")
set(CMAKE_CXX_FLAGS "-Wall -g -O2")
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
add_library(cppsproto ${LIB_SRC})

project(demo)
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(EXE_SRC demo.cpp addressbookmessage.cpp)
set(CMAKE_CXX_FLAGS "-Wall -g -O2")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
add_executable(demo ${EXE_SRC})
target_link_libraries(demo libcppsproto.a)

