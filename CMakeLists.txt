cmake_minimum_required(VERSION 3.25.1)

project(PhotoBatch)

add_executable(PhotoBatch
"photo-batch.cpp"
"argument-parser.h"
"argument-parser.cpp"
"utils.h"
"utils.cpp"
"mode.h"
"mode.cpp"
"rename-mode.h"
"rename-mode.cpp"
"convert-mode.h"
"convert-mode.cpp"
"resize-mode.h"
"resize-mode.cpp"
"scale-mode.h"
"scale-mode.cpp"
)

set_target_properties(PhotoBatch PROPERTIES CXX_STANDARD 17)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Disable quiet mode on FetchContent" FORCE)
include(FetchContent)

add_subdirectory(tests)
add_subdirectory(external)

target_link_libraries(PhotoBatch PRIVATE stb)