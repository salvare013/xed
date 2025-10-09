cmake_minimum_required(VERSION 3.16)

project(xed VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Concurrent)

aux_source_directory(src PROJECT_SOURCES)

if(WIN32)
    add_executable(xed ${PROJECT_SOURCES} res.qrc resfiles/ico.rc)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resfiles/xed.ico ${CMAKE_CURRENT_BINARY_DIR}/xed.ico COPYONLY)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(xed PROPERTIES WIN32_EXECUTABLE FALSE)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(xed PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()

else()
    add_executable(xed ${PROJECT_SOURCES} res.qrc)
endif()

target_link_libraries(xed PRIVATE Qt5::Widgets)
target_link_libraries(xed PRIVATE Qt5::Concurrent)
