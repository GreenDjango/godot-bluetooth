cmake_minimum_required(VERSION 3.1.0)

project(godot-bluetooth)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SRCS
    "src/main.cpp"
)

set(INCS
    "include/"
)

add_executable(gblue.out ${SRCS})
target_include_directories(gblue.out PRIVATE ${INCS})
target_compile_options(gblue.out PRIVATE -Werror -Wall -Wextra)

target_link_libraries(gblue.out Qt5::Widgets)