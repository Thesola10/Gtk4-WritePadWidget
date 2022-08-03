configure_file(WPDictionaries.h.in WPDictionaries.gen.h @ONLY)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/widget)

add_library(WritePadWidget STATIC)
target_sources(WritePadWidget
        PRIVATE WritePadWidget.h
        PRIVATE WritePadWidget.c
        PRIVATE WritePadWidget.impl.h
        PRIVATE WritePadWidget.impl.c)
target_link_libraries(WritePadWidget
        PRIVATE ${GTK4_LIBRARIES} m stdc++ WritePadLib pthread)

add_dependencies(WritePadWidget writepad)


install(TARGETS WritePadWidget DESTINATION libs)
install(FILES WritePadWidget.h DESTINATION include)
install(FILES ${source_dir}/Dictionaries/*.dct DESTINATION ${CMAKE_INSTALL_PREFIX}/share/WritePad/Dictionaries)
