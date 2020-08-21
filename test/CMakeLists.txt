message(STATUS "Building tests...")

add_library(catch_main STATIC catch_main.cpp)

add_executable(tests tests.cpp)
target_compile_features(tests PRIVATE cxx_std_17)

find_package(hamarr CONFIG REQUIRED)
target_link_libraries(tests PRIVATE hamarr::hamarr catch_main)
