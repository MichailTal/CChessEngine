cmake_minimum_required(VERSION 3.10)
project(cchess_engine)

# Set compiler flags for debugging
set(CMAKE_C_FLAGS_DEBUG "-g")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# List of source files
set(SOURCES
    cchess_engine.c
    init.c
    bitboard.c
    hashkey.c
    data.c
    board.c
    attack.c
    io.c
    movegen.c
    validate.c
    makemove.c
    perft.c
    search.c
)

# Define the executable
add_executable(cchess_engine ${SOURCES})

# Enable debugging if requested
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(cchess_engine PRIVATE -g)
endif()
