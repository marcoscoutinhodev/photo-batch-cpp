FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(
  TestArgumentParser
  "test-argument-parser.cpp"
  "${CMAKE_SOURCE_DIR}/argument-parser.cpp"
  "${CMAKE_SOURCE_DIR}/utils.cpp"
)

target_include_directories(TestArgumentParser PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(TestArgumentParser PRIVATE gtest_main)