
# find_package(UtilsLib REQUIRED)

# Create a library
file(GLOB SOURCES *.cpp *.h)
# add_definitions(-include Common.h)
add_library(TradeCore SHARED ${SOURCES})

find_package(Toybox)

target_include_directories(TradeCore  PUBLIC
 #not really understanding, not exposing our path to someone who will install the lib?
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)
# target_link_libraries(UtilsLib PRIVATE Boost::asio)

# install libraries
install(TARGETS TradeCore
  EXPORT TradeCoreTargets
  ARCHIVE DESTINATION lib # static libs
  LIBRARY DESTINATION lib # dynamic
  RUNTIME DESTINATION bin # dll libs
)

# install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/TradeCore
    FILES_MATCHING PATTERN "*.h"
)

# install cmake files, for find_package to work
install(EXPORT TradeCoreTargets
  FILE TradeCoreConfig.cmake
  DESTINATION lib/cmake/TradeCore
)