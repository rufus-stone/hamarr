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

find_package(hamarr CONFIG REQUIRED)

find_package(spdlog REQUIRED)
target_link_libraries(hamarr_examples PRIVATE example_options example_warnings
                                              hamarr::hamarr spdlog::spdlog)
