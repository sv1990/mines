cmake_minimum_required(VERSION 3.9)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
  message(STATUS "Using ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif ()

# Let AUTOMOC and AUTOUIC process .hh files.
if (POLICY CMP0100)
  cmake_policy(SET CMP0100 NEW)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6Widgets REQUIRED HINTS /usr/local/opt/qt/ )
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
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(
    mines
    PUBLIC "-Wall;-Wextra;-pedantic;-Wconversion;-Wsign-conversion;-Wshadow"
  )
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(mines PUBLIC -Wno-assume)
endif ()
target_compile_features(mines PUBLIC cxx_std_17)
target_link_libraries(
  mines PRIVATE range-v3::range-v3 fmt::fmt Qt6::Widgets Boost::serialization
)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
  message(STATUS "Using -march=native")
  target_compile_options(mines PRIVATE -march=native)
endif ()

include(CheckIPOSupported)
check_ipo_supported(RESULT HAVE_IPO)
if (HAVE_IPO)
  message(STATUS "Using lto")
  set_property(TARGET mines PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

find_program(HAVE_LLD lld)
if (HAVE_LLD AND NOT (CMAKE_CXX_COMPILER_ID MATCHES GNU AND HAVE_IPO))
  message(STATUS "Using lld")
  target_link_options(mines PUBLIC -fuse-ld=lld)
endif ()

install(TARGETS mines RUNTIME DESTINATION bin)
