cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

project(RPI_SerialReader VERSION 0.1.0)

include(CTest)
enable_testing()

# library

add_library(RPI_SerialReader 
    src/RPI_SerialReader.cpp
    src/Message.cpp
)

target_include_directories( 
    RPI_SerialReader PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# test executable
add_executable(RPI_SerialReader_Test main.cpp)
target_include_directories( 
    RPI_SerialReader_Test PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(RPI_SerialReader_Test RPI_SerialReader)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
