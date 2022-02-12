cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    main_windows.cpp
    setting.cpp
    restreminder.qrc
)

INCLUDE_DIRECTORIES(.)


add_executable(restreminder ${SOURCES} )


target_link_libraries(restreminder PRIVATE Qt6::Widgets)
