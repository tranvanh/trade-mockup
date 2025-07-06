# Create a library
file(GLOB SOURCES *.cpp *.h)
# add_definitions(-include Common.h)
add_library(TradeLib SHARED ${SOURCES})

# Link the static library 'UtilsLib'
target_link_libraries(TradeLib PRIVATE UtilsLib)

# Expose headers to other targets
target_include_directories(TradeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})