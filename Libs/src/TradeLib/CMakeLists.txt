
# find_package(UtilsLib REQUIRED)

# Create a library
file(GLOB SOURCES *.cpp *.h)
# add_definitions(-include Common.h)
add_library(TradeLib SHARED ${SOURCES})

# Link the static library 'UtilsLib'
# Locate UtilsLib
target_link_libraries(TradeLib PRIVATE UtilsLib)

target_include_directories(TradeLib  PUBLIC
 #not really understanding, not exposing our path to someone who will install the lib?
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)
# target_link_libraries(UtilsLib PRIVATE Boost::asio)

# install libraries
install(TARGETS TradeLib
  EXPORT TradeLibTargets
  ARCHIVE DESTINATION lib # static libs
  LIBRARY DESTINATION lib # dynamic
  RUNTIME DESTINATION bin # dll libs
)

# install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/TradeLib
    FILES_MATCHING PATTERN "*.h"
)

# install cmake files, for find_package to work
install(EXPORT TradeLibTargets
  FILE TradeLibConfig.cmake
  DESTINATION lib/cmake/TradeLib
)