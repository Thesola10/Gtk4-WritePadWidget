cmake_minimum_required(VERSION 2.6)
project(Writepad-Demo C)
include(ExternalProject)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

ExternalProject_Add(writepad
    GIT_REPOSITORY https://github.com/phatware/WritePad-Handwriting-Recognition-Engine
    GIT_SHALLOW 1
    GIT_PROGRESS 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -C Linux
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
    )

ExternalProject_Get_Property(writepad source_dir)

INCLUDE_DIRECTORIES(${GTK4_INCLUDE_DIRS} ${source_dir}/Linux/include)
LINK_DIRECTORIES(${GTK4_LIBRARY_DIRS} ${source_dir}/Linux/Lib)

ADD_DEFINITIONS(${GTK4_CFLAGS_OTHER})

add_subdirectory(widget)

add_executable(WritePadWidget-demo main.c)

target_link_libraries(WritePadWidget-demo ${GTK4_LIBRARIES} WritePadWidget)

file(COPY ${source_dir}/Dictionaries DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
