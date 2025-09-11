
# Create a lib
file(GLOB SOURCES *.cpp *.h)
add_library(UtilsLib STATIC ${SOURCES})

target_include_directories(UtilsLib  PUBLIC
 #not really understanding, not exposing our path to someone who will install the lib?
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

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