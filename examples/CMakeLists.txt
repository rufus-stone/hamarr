message(STATUS "Building examples...")

add_executable(hamarr_examples main.cpp)

# Link this 'library' to set the c++ standard / compile-time options and enable
# sanitzers
add_library(example_options INTERFACE)
target_compile_features(example_options INTERFACE cxx_std_17)
include(../cmake/Sanitizers.cmake)
enable_sanitizers(example_options)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(example_warnings INTERFACE)
include(../cmake/CompilerWarnings.cmake)
set_project_warnings(example_warnings)

target_include_directories(hamarr_examples PRIVATE ../include)

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

target_link_libraries(hamarr_examples PRIVATE example_options example_warnings OpenSSL::SSL spdlog::spdlog)
