cmake_minimum_required(VERSION 3.5)

project(3DReviewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -fsanitize=address,undefined")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

add_subdirectory(gif_lib/QtGifImage)

set(PROJECT_SOURCES
        main.cc

        view/coloradapter.cc
        view/coloradapter.h
        view/lineedit_adapter.cc
        view/lineedit_adapter.h
        view/scrollbar_adapter.cc
        view/scrollbar_adapter.h
        view/glwidget.h
        view/glwidget.cc
        view/view.cc
        view/view.h
        view/view.ui

        controller/viewer_controller.h

        model/object_builder.cc
        model/parser.cc
        model/viewer.cc
        model/transformer.cc
        model/object.h
        model/object_builder.h
        model/parser.h
        model/viewer.h
        model/transformer.h
        model/exception.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3DReviewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 3DReviewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3DReviewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3DReviewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

#set(LINK_LIBRARIES
#        QtGifImage::QtGifImage
#)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::OpenGLWidgets)
target_link_libraries(${PROJECT_NAME} PRIVATE QtGifImage::QtGifImage)

set_target_properties(3DReviewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3DReviewer)
endif()
