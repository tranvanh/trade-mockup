file(GLOB SOURCES *.cpp)
add_executable(TradeLib.Sandbox ${SOURCES})
target_link_libraries(TradeLib.Sandbox PRIVATE
    UtilsLib TradeLib
)
target_link_libraries(TradeLib.Sandbox PRIVATE TradeLib UtilsLib)

