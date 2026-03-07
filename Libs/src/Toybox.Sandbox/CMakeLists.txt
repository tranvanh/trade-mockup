file(GLOB SOURCES *.cpp)
add_executable(Toybox.Sandbox ${SOURCES})
target_link_libraries(Toybox.Sandbox PRIVATE
    Toybox
)
target_link_libraries(Toybox.Sandbox PRIVATE Toybox)

