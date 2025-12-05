file(GLOB SOURCES *.cpp)
add_executable(UtilsLib.Sandbox ${SOURCES})
target_link_libraries(UtilsLib.Sandbox PRIVATE
    UtilsLib
)
target_link_libraries(UtilsLib.Sandbox PRIVATE UtilsLib)

