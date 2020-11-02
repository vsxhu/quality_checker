aux_source_directory(. DIR_SRCS)

add_executable(quality_checker ${DIR_SRCS} Checker.cpp Checker.h)
target_link_libraries(quality_checker
        pvbsim
        ${BLAS_LIBRARIES}
        ${PNG_LIBRARIES}
        ${ZLIB_LIBRARIES}
        -fopenmp
        )
add_definitions("-DDEBUGMSG -DDEBUG -std=c++11 -O3")
