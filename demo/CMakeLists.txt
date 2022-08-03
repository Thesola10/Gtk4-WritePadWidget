
add_executable(WritePadWidget-demo)

target_sources(WritePadWidget-demo
        PRIVATE main.c)
target_link_libraries(WritePadWidget-demo
        PRIVATE ${GTK4_LIBRARIES}
        PRIVATE WritePadWidget)
target_include_directories(WritePadWidget-demo
        PRIVATE ${CMAKE_SOURCE_DIR}/widget)
