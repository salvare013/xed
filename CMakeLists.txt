cmake_minimum_required(VERSION 3.16)

project(xed VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    bimap.hpp
    mainwindow.cpp mainwindow.h
    menubar.cpp menubar.h
    statusbar.cpp statusbar.h
    tabwidget.cpp tabwidget.h
    textedit.cpp textedit.h
    clickablelabel.cpp clickablelabel.h
    selector.cpp selector.h
    res.qrc
)

if(WIN32)
    add_executable(xed ${PROJECT_SOURCES} resfiles/ico.rc)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resfiles/xed.ico ${CMAKE_CURRENT_BINARY_DIR}/xed.ico COPYONLY)
else()
    add_executable(xed ${PROJECT_SOURCES})
endif()

target_link_libraries(xed PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(xed PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
