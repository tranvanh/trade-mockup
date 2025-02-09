cmake_minimum_required(VERSION 3.10)

project(Trade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2 SDL2 SDL2_image)

file(GLOB src *.cpp)

include_directories( ${PROJECT_SOURCE_DIR})
# include(Common.h)
add_executable(Trade main.cpp)
target_sources(Trade PRIVATE ${src})

# target_link_libraries(Trade)
