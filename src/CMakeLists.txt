cmake_minimum_required(VERSION 3.8)

set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_executable(mines main.cc gameboard.cc pixmap.cc field.cc timer.cc highscore.cc images.qrc)
target_compile_options(mines PUBLIC "-Wall;-Wextra;-pedantic")
target_compile_features(mines PUBLIC cxx_std_20)
target_link_libraries(mines stdc++fs Qt5::Widgets)
