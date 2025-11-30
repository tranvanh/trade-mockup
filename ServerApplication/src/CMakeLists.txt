include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)

set(LIB_PATH ${CMAKE_SOURCE_DIR}/../includeLibs)

set(CMAKE_PREFIX_PATH ${LIB_PATH})

find_package(UtilsLib)
find_package(TradeLib)

file(GLOB SOURCES *.cpp *.h)
add_executable(serverApp ${SOURCES})

target_compile_options(serverApp PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(serverApp PRIVATE UtilsLib TradeLib nlohmann_json::nlohmann_json)

# Install your executable
install(TARGETS serverApp
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            GROUP_EXECUTE GROUP_READ
            WORLD_EXECUTE WORLD_READ
)

# Install other files (optional)
install(FILES 
    ${LIB_PATH}/lib/libTradeLib.so
    DESTINATION lib
)
