cmake_minimum_required(VERSION 3.16)

project(godot-bluetooth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Bluetooth)
# find_package(Qt6 REQUIRED COMPONENTS Bluetooth Core Widgets)

set(SRCS
    "src/main.cpp"
)

set(INCS
    "include/"
)

add_executable(gblue ${SRCS})
# target_include_directories(gblue PRIVATE ${INCS})
target_compile_options(gblue PRIVATE -Wall -Wextra -g3)

target_link_libraries(gblue PUBLIC
    Qt::Bluetooth
)

set_target_properties(gblue PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if (APPLE)
    if (IOS)
        set_target_properties(gblue PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
        )
    else()
        set_target_properties(gblue PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.cmake.macos.plist"
        )
    endif()
endif()

set(INSTALL_DIR "bin")

install(TARGETS gblue
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)