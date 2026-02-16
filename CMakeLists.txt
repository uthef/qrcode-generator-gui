cmake_minimum_required(VERSION 3.16)

project(qr-code-generator-gui VERSION 1.0.0 LANGUAGES CXX)
set(APP_TITLE "QR Code Generator GUI")

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

if (WIN32)
    set(WIN_RESOURCE_FILE resources/win.rc)
endif()

add_executable(${PROJECT_NAME}
    ${WIN_RESOURCE_FILE}

    src/lib/nayuki/qr-code-generator/qrcodegen.cpp

    src/qr/qrcodegenerator.cpp

    resources/res.qrc
    src/main.cpp

    src/layouts/mainwindowlayout.hpp
    src/layouts/mainwindowlayout.cpp
    src/widgets/mainwindow.hpp
    src/widgets/mainwindow.cpp

    src/widgets/popupdialog.hpp
    src/widgets/popupdialog.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CMAKE_CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    AUTORCC ON
)

if (WIN32)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC 
    APP_TITLE="${APP_TITLE}"
    APP_VERSION="${PROJECT_VERSION}"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Svg
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    src/lib
    src
)

include(GNUInstallDirs)