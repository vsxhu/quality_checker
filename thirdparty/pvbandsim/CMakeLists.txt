file(GLOB SOURCES
        *.cpp
        *.c
        )
file(GLOB HEADERS
        *.h
        )
add_compile_options(-fopenmp -O3)
add_library(pvbsim ${SOURCES})
set(CMAKE_CXX_STANDARD 11)
