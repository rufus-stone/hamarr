cmake_minimum_required(VERSION 3.0.0)
project(hamarr VERSION 0.9.0)

include(CTest)
enable_testing()

include_directories(include)

add_executable(hamarr main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_compile_features(hamarr PUBLIC cxx_std_17)
