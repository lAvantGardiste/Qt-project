cmake_minimum_required(VERSION 3.16)
project(parachute_encoder VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

add_executable(parachute_encoder
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/model/message.cpp
    src/model/message.h
    src/view/parachuteview.cpp
    src/view/parachuteview.h
)

target_link_libraries(parachute_encoder PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
) 