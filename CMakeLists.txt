cmake_minimum_required(VERSION 3.5)

project(diskspacetool VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES

    aboutdialog.h aboutdialog.cpp aboutdialog.ui
    choosedirectoryform.h choosedirectoryform.cpp
    scanform.h scanform.cpp
    applicationstate.h applicationstate.cpp
    scanner.h scanner.cpp
    resultsform.h resultsform.cpp
    toast.h
    util.h util.cpp
    guard.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

qt_add_executable(diskspacetool MANUAL_FINALIZATION ${PROJECT_SOURCES}
    scanentrymodel.h scanentrymodel.cpp
    aligndelegate.h aligndelegate.cpp)

target_link_libraries(diskspacetool PRIVATE Qt6::Widgets)
qt_finalize_executable(diskspacetool)

