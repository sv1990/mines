cmake_minimum_required(VERSION 3.8)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(range-v3 REQUIRED)
if ("${range-v3_VERSION}" VERSION_LESS "0.9.0")
  message(FATAL_ERROR "mines requires range-v3 version 0.9.0 or above")
endif ()
find_package(fmt REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)

add_executable(
  mines
  main.cc
  gameboard.cc
  pixmap.cc
  field.cc
  highscore.cc
  mines.cc
  bomb_count.cc
  entry.cc
  timer.cc
  images.qrc
)
target_compile_options(
  mines
  PUBLIC "-Wall;-Wextra;-pedantic;-Wconversion;-Wsign-conversion;-Wshadow"
)
target_compile_features(mines PUBLIC cxx_std_20)
target_link_libraries(
  mines PRIVATE stdc++fs range-v3::range-v3 fmt Qt5::Widgets
                Boost::serialization
)

install(TARGETS mines RUNTIME DESTINATION bin)
