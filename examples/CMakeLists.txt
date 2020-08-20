add_executable(hamarr_examples main.cpp)
#target_compile_features(hamarr_examples PRIVATE cxx_std_17)

# Link this 'library' to set the c++ standard / compile-time options and enable sanitzers
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
include(../cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
include(../cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

find_package(hamarr CONFIG REQUIRED)
target_link_libraries(hamarr_examples PRIVATE project_options project_warnings hamarr::hamarr)