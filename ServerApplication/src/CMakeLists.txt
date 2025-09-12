include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)

set(CMAKE_PREFIX_PATH "/home/tranvanh/coding/trade-mockup/includeLibs")

find_package(UtilsLib)
find_package(TradeLib)

file(GLOB SOURCES *.cpp *.h)
add_executable(serverApp ${SOURCES})

target_compile_options(serverApp PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(serverApp PRIVATE UtilsLib TradeLib nlohmann_json::nlohmann_json)
