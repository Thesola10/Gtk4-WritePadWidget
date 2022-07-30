cmake_minimum_required(VERSION 2.6)
project(Writepad-Demo C)
include(ExternalProject)

ExternalProject_Add(writepad
    GIT_REPOSITORY https://github.com/phatware/WritePad-Handwriting-Recognition-Engine
    GIT_SHALLOW 1
    GIT_PROGRESS 1
    BUILD_COMMAND make -C Linux
    BUILD_IN_SOURCE 1
    )
