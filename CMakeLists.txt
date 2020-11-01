cmake_minimum_required(VERSION 3.5)
project(quality_checker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/runtime)

find_package(BLAS REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
add_definitions(-Wunused-result)

include_directories(
        thirdparty/pvbandsim
        src
        ${BLAS_INCLUDE_DIRECTORIES}
        ${ZLIB_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIRS}
)
add_subdirectory(thirdparty/pvbandsim)
add_subdirectory(src)