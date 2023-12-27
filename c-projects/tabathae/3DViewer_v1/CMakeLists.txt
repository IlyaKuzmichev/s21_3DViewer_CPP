cmake_minimum_required(VERSION 3.5)

project(3DViewer_v1 VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt7 Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ../object_builder.h
        ../object_parser.h
        ../object_builder.c
        ../object_parser.c
        ../object.c
        ../object.h
        myglwidget.h
        myglwidget.cpp
        ./QtGifImage-master/src/3rdParty/giflib/dgif_lib.c
        ./QtGifImage-master/src/3rdParty/giflib/egif_lib.c
        ./QtGifImage-master/src/3rdParty/giflib/gif_err.c
        ./QtGifImage-master/src/3rdParty/giflib/gif_lib.h
        ./QtGifImage-master/src/3rdParty/giflib/gif_hash.c
        ./QtGifImage-master/src/3rdParty/giflib/gifalloc.c
        ./QtGifImage-master/src/3rdParty/giflib/quantize.c
        ./QtGifImage-master/src/gifimage/qgifglobal.h
        ./QtGifImage-master/src/gifimage/qgifimage.h
        ./QtGifImage-master/src/gifimage/qgifimage_p.h
        ./QtGifImage-master/src/gifimage/qgifimage.cpp
        images.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3DViewer_v1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 3DViewer_v1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3DViewer_v1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3DViewer_v1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(3DViewer_v1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(3DViewer_v1 PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

set_target_properties(3DViewer_v1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_finalize_executable(3DViewer_v1)
endif()
