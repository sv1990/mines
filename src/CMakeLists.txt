cmake_minimum_required(VERSION 3.8)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

include(CheckCXXCompilerFlag)

find_package(Qt5Widgets REQUIRED)
find_package(range-v3 REQUIRED)
if ("${range-v3_VERSION}" VERSION_LESS "0.9.0")
  message(FATAL_ERROR "mines requires range-v3 version 0.9.0 or above")
endif ()
find_package(fmt REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)

option(USE_CLANG_TIDY "Use Clang-Tidy" OFF)
option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" OFF)
find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
)

if (USE_CLANG_TIDY AND CLANG_TIDY_EXE)
  if (CLANG_TIDY_FIX)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" "-fix")
  else ()
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif ()
endif ()

option(USE_CONCEPTS "Add the concepts ts" ON)
check_cxx_compiler_flag("-fconcepts" HAVE_CONCEPTS)

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
  mines
  PRIVATE
    stdc++fs
    range-v3
    fmt
    Qt5::Widgets
    Boost::serialization
)
if (USE_CONCEPTS AND HAVE_CONCEPTS AND NOT USE_CLANG_TIDY)
  target_compile_options(mines PRIVATE -fconcepts)
endif ()

install(TARGETS mines RUNTIME DESTINATION bin)
