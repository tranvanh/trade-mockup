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
add_executable(clientApp ${SOURCES})

target_compile_options(clientApp PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(clientApp PRIVATE UtilsLib TradeLib)
target_link_libraries(clientApp PRIVATE Boost::asio)
