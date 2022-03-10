cmake_minimum_required(VERSION 3.5)

project(game-model VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/gamewindow.cpp
    src/player.cpp
    src/map.cpp
    src/food.cpp
    src/moving.cpp

    include/gamewindow.h
    include/food.h
    include/moving.h
    include/map.h
    gamewindow.ui
    include/player.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(game-model
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    add_executable(game-model
            ${PROJECT_SOURCES}
        )
endif()

target_link_libraries(game-model PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(game-model PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(game-model)
endif()
