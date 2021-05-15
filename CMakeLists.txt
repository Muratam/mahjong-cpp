cmake_minimum_required(VERSION 3.1.1)
project(mahjong-cpp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

include_directories(/usr/local/include)
# なぜかemcmakeではうまく動作しないので↑でboostを読む
# find_package(Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})

# Fetch rapidjson
include(FetchContent)
FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG v1.1.0)
FetchContent_GetProperties(rapidjson)
if(NOT rapidjson_POPULATED)
  FetchContent_Populate(rapidjson)
endif()
include_directories(${rapidjson_SOURCE_DIR}/include)

# spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.x)
FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif()

# mahjong
include_directories(src)

# emscripten
option(BUILD_EMSCRIPTEN "build emscripten test." ON)
if(BUILD_EMSCRIPTEN)
  add_subdirectory(src/emscripten)
endif()
