cmake_minimum_required(VERSION 3.8)

set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(range-v3 REQUIRED)
find_package(fmt REQUIRED)

add_executable(mines main.cc gameboard.cc pixmap.cc field.cc highscore.cc mines.cc images.qrc)
target_compile_options(mines PUBLIC "-Wall;-Wextra;-pedantic;-Wconversion;-Wsign-conversion;-Wshadow")
target_compile_features(mines PUBLIC cxx_std_20)
target_link_libraries(mines stdc++fs range-v3 fmt Qt5::Widgets)

install(TARGETS mines
        RUNTIME DESTINATION bin)
