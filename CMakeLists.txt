cmake_minimum_required(VERSION 3.5)

project(openglqt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        glad.c
        KHR/khrplatform.h
        myopenglwidget.h myopenglwidget.cpp
        fwworker.h fwworker.cpp
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(openglqt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(openglqt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(openglqt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(openglqt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets OpenGL::GL glfw ${CMAKE_DL_LIBS})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(openglqt)
endif()
