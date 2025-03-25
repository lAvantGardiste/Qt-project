cmake_minimum_required(VERSION 3.16)
project(parachute VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

# Define source files
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/model/message.cpp
    src/model/message.h
    src/view/parachuteview.cpp
    src/view/parachuteview.h
)

# Define translation files
set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/parachute_en.ts)

# Simple translation handling
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

# Add executable
qt_add_executable(parachute
    ${PROJECT_SOURCES}
    resources.qrc
)

# Link against Qt libraries
target_link_libraries(parachute PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Ensure the build directory is used for generated files
set_target_properties(parachute PROPERTIES
    AUTOMOC_MOC_OPTIONS "-I${CMAKE_CURRENT_BINARY_DIR}"
    AUTOUIC_OPTIONS "-I${CMAKE_CURRENT_BINARY_DIR}"
) 