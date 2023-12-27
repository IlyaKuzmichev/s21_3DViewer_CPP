cmake_minimum_required(VERSION 3.5)

project(s21_3DViewer VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    myglwidget.h
    myglwidget.cpp
    myobject.h
    myobject.cpp

# backend
    ../backend/parser.h
    ../backend/3d_viewer.h
    ../backend/affine_transformations.c
    ../backend/parser.c

    ObjectParameters.h

# Adapters
    coloradapter.h
    coloradapter.cpp
    scrollbaradapter.h
    scrollbaradapter.cpp
    lineeditadapter.h
    lineeditadapter.cpp

# QTGifImage
    ./QtGifImage/src/gifimage/qgifimage.h
    ./QtGifImage/src/gifimage/qgifimage.cpp
    ./QtGifImage/src/gifimage/qgifglobal.h
    ./QtGifImage/src/gifimage/qgifimage_p.h
    ./QtGifImage/src/3rdParty/giflib/gif_lib.h
    ./QtGifImage/src/3rdParty/giflib/dgif_lib.c
    ./QtGifImage/src/3rdParty/giflib/egif_lib.c
    ./QtGifImage/src/3rdParty/giflib/gif_err.c
    ./QtGifImage/src/3rdParty/giflib/gif_font.c
    ./QtGifImage/src/3rdParty/giflib/gif_hash.c
    ./QtGifImage/src/3rdParty/giflib/gifalloc.c
    ./QtGifImage/src/3rdParty/giflib/quantize.c

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(s21_3DViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET s21_3DViewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(s21_3DViewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(s21_3DViewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(s21_3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(s21_3DViewer PRIVATE Qt6::OpenGLWidgets)

set_target_properties(s21_3DViewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(s21_3DViewer)
endif()
