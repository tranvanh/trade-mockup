include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.3.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cxxopts)

find_package(Boost REQUIRED)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/../includeLibs)

find_package(Toybox)
find_package(TradeCore)

file(GLOB SOURCES *.cpp *.h)
add_executable(clientApp ${SOURCES})

target_compile_options(clientApp PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(clientApp PRIVATE Toybox TradeCore nlohmann_json::nlohmann_json cxxopts Boost::headers)
