cmake_minimum_required(VERSION 3.15...3.18)

project(
  hamarr
  VERSION 0.9.0
  DESCRIPTION "A C++ toolbox for data manipulation."
  HOMEPAGE_URL "https://github.com/rufus-stone/hamarr"
  LANGUAGES CXX)

# Disallow in-source builds
include(cmake/PreventInSourceBuilds.cmake)

include(GNUInstallDirs) # This provides GNU standard installation directories,
                        # so we don't have to hard code anything

include(CTest) # This adds the BUILD_TESTING option (ON by default)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_TOPLEVEL_PROJECT TRUE)
else()
  set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(INSTALL_LIBRARY
       "Enable installation of Hamarr library into default locations"
       ${IS_TOPLEVEL_PROJECT})
option(BUILD_HAMARR_TESTS "Build Hamarr tests" ${IS_TOPLEVEL_PROJECT})
option(BUILD_HAMARR_EXAMPLES "Build Hamarr examples" ${IS_TOPLEVEL_PROJECT})

add_library(${PROJECT_NAME} INTERFACE)

# Add an alias so the project can be used with add_subdirectory()
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUInstallDirs)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# We require C++17
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if(BUILD_TESTING AND BUILD_HAMARR_TESTS)
  enable_testing()
  message("Building tests...")
  add_subdirectory(test)
endif()

if(BUILD_HAMARR_EXAMPLES)
  message("Building examples...")
  add_subdirectory(examples)
endif()

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE OpenSSL::SSL)

# Install locations are provided by GNUInstallDirs
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

if(INSTALL_LIBRARY)
  message("Installing library...")
  install(
    EXPORT ${PROJECT_NAME}_Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/hamarr
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

include(CPack)

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message(STATUS "IS_TOPLEVEL_PROJECT: ${IS_TOPLEVEL_PROJECT}")
message(STATUS "CMAKE_INSTALL_DATAROOTDIR: ${CMAKE_INSTALL_DATAROOTDIR}")
