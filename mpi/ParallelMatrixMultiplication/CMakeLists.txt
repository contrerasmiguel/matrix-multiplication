cmake_minimum_required(VERSION 3.5)
project(ParallelMatrixMultiplication)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

set(SOURCE_FILES src/Matrix.cpp src/Matrix.h src/Multiplication.cpp src/Multiplication.h src/Input.cpp src/Input.h src/Master.cpp src/Master.h src/Worker.cpp src/Worker.h src/main.cpp src/main.h src/Node.h src/Parallel.cpp src/Parallel.h)
add_executable(PMM ${SOURCE_FILES})

# C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Require MPI for this project:
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS}")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS}")
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(PMM ${MPI_LIBRARIES})
