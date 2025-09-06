set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)

# include(FetchContent)
# FetchContent_Declare(
#   Boost
#   GIT_REPOSITORY https://github.com/boostorg/boost.git
#   GIT_TAG boost-1.88.0
#   GIT_SHALLOW TRUE
# )
# FetchContent_MakeAvailable(Boost)

# Create a lib
file(GLOB SOURCES *.cpp *.h)
# add_definitions(-include Common.h)
add_library(UtilsLib STATIC ${SOURCES})

target_include_directories(UtilsLib  PUBLIC
 #not really understanding, not exposing our path to someone who will install the lib?
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)
# target_link_libraries(UtilsLib PRIVATE Boost::asio)

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