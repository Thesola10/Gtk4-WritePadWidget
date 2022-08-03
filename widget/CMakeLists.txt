
add_library(WritePadWidget STATIC)
target_sources(WritePadWidget
        PRIVATE WritePadWidget.h
        PRIVATE WritePadWidget.c
        PRIVATE WritePadWidget.impl.h
        PRIVATE WritePadWidget.impl.c)
target_link_libraries(WritePadWidget
        PRIVATE ${GTK4_LIBRARIES} m stdc++ WritePadLib pthread)

install(TARGETS WritePadWidget DESTINATION libs)
install(FILES WritePadWidget.h DESTINATION include)
