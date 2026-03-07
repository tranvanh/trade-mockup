set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-36-0
)
FetchContent_MakeAvailable(asio)

find_package(Threads REQUIRED)

# Create a lib
file(GLOB SOURCES *.cpp *.h)
add_library(Toybox SHARED ${SOURCES})

target_include_directories(Toybox PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${asio_SOURCE_DIR}/asio/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(Toybox PUBLIC
        $<$<CONFIG:Release>:RELEASE_BUILD>
        $<$<CONFIG:ASSERT>:ASSERT_BUILD>
        $<$<CONFIG:DEBUG>:DEBUG_BUILD>
        ASIO_STANDALONE
)
target_link_libraries(Toybox PUBLIC Threads::Threads)

# install libraries
install(TARGETS Toybox
        EXPORT ToyboxTargets
        ARCHIVE DESTINATION lib # static libs
        LIBRARY DESTINATION lib # dynamic
        RUNTIME DESTINATION bin # dll libs
)

# install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/Toybox
        FILES_MATCHING PATTERN "*.h"
)

# install cmake files, for find_package to work
install(EXPORT ToyboxTargets
        FILE ToyboxConfig.cmake
        DESTINATION lib/cmake/Toybox
)