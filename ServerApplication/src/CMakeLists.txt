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

find_package(UtilsLib)
find_package(TradeLib)

file(GLOB SOURCES *.cpp *.h)
add_executable(serverApp ${SOURCES})

target_compile_options(serverApp PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(serverApp PRIVATE UtilsLib TradeLib)
target_link_libraries(serverApp PRIVATE Boost::asio)