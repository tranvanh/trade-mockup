add_subdirectory(TradeLib)
add_subdirectory(UtilsLib)
if(BUILD_TESTS)
    add_subdirectory(UtilsLib.Test)
endif()

