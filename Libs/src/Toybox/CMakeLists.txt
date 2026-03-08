set(CMAKE_POSITION_INDEPENDENT_CODE ON)


find_package(Boost REQUIRED)

# Create Toybox library
file(GLOB SOURCES *.cpp *.h)
add_library(Toybox SHARED ${SOURCES})

target_link_libraries(Toybox Boost::headers)

target_include_directories(Toybox PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(Toybox PUBLIC
        $<$<CONFIG:Release>:RELEASE_BUILD>
        $<$<CONFIG:ASSERT>:ASSERT_BUILD>
        $<$<CONFIG:DEBUG>:DEBUG_BUILD>
)

# Include Toybox headers
target_include_directories(Toybox
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# Install Toybox headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/Toybox
        FILES_MATCHING PATTERN "*.h"
)

# Install library and export
install(TARGETS Toybox
        EXPORT ToyboxTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

install(EXPORT ToyboxTargets
        FILE ToyboxConfig.cmake
        DESTINATION lib/cmake/Toybox
)