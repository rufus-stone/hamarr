message(STATUS "Building hamarr_tests...")

add_library(hmr_catch_main STATIC catch_main.cpp)

add_executable(hamarr_tests tests.cpp)
target_compile_features(hamarr_tests PRIVATE cxx_std_17)

target_include_directories(hamarr_tests PRIVATE ../include)

find_package(OpenSSL REQUIRED)
find_package(spdlog QUIET)
if(NOT spdlog_FOUND)
  message("Failed to find spdlog! Fetching it instead...")

  include(FetchContent)

  FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.8.1)

  FetchContent_MakeAvailable(spdlog)
endif()

target_link_libraries(hamarr_tests PRIVATE hmr_catch_main OpenSSL::SSL spdlog::spdlog)
