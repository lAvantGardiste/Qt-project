cmake_minimum_required(VERSION 3.16)
project(parachute VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup()

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/model/message.cpp
    src/model/message.h
    src/view/parachuteview.cpp
    src/view/parachuteview.h
)

# Translation files
set(TS_FILES translations/parachute_en.ts)

qt_add_executable(parachute ${PROJECT_SOURCES})
qt_add_translations(parachute TS_FILES ${TS_FILES})

target_link_libraries(parachute PRIVATE Qt6::Widgets) 