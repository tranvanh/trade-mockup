set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.88.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Boost)

# Create a lib
file(GLOB SOURCES *.cpp *.h)
# add_definitions(-include Common.h)
add_library(UtilsLib STATIC ${SOURCES})

target_include_directories(UtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(UtilsLib PRIVATE Boost::asio)
