set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-36-0
)
FetchContent_MakeAvailable(asio)

# Create a lib
file(GLOB SOURCES *.cpp *.h)
add_library(UtilsLib SHARED ${SOURCES})

target_include_directories(UtilsLib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${asio_SOURCE_DIR}/asio/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(UtilsLib PUBLIC
        $<$<CONFIG:Release>:RELEASE_BUILD>
        $<$<CONFIG:ASSERT>:ASSERT_BUILD>
        $<$<CONFIG:DEBUG>:DEBUG_BUILD>
        ASIO_STANDALONE
)

# install libraries
install(TARGETS UtilsLib
        EXPORT UtilsLibTargets
        ARCHIVE DESTINATION lib # static libs
        LIBRARY DESTINATION lib # dynamic
        RUNTIME DESTINATION bin # dll libs
)

# install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/UtilsLib
        FILES_MATCHING PATTERN "*.h"
)

# install cmake files, for find_package to work
install(EXPORT UtilsLibTargets
        FILE UtilsLibConfig.cmake
        DESTINATION lib/cmake/UtilsLib
)