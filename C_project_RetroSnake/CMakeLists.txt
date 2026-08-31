cmake_minimum_required(VERSION 3.16)
project(SnakeGame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(snake_game ${SOURCES})

target_link_libraries(snake_game PRIVATE 
    mingw32 
    SDL2main 
    SDL2 
    SDL2_gfx
)

add_custom_command(TARGET snake_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:snake_game>/data
)