cmake_minimum_required(VERSION 3.15...3.18)

#[[
-------------
Project Setup
-------------
]]

project(
  hamarr
  VERSION 1.1.1
  DESCRIPTION "A C++ toolbox for data manipulation."
  HOMEPAGE_URL "https://github.com/rufus-stone/hamarr"
  LANGUAGES CXX)

include(cmake/PreventInSourceBuilds.cmake) # No in-source builds

include(GNUInstallDirs) # Provides default install locations

# Add the library and library alias
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# We require C++17
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)


target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

#[[
------------
Dependencies
------------
]]

find_package(OpenSSL REQUIRED) # TODO: Make this optional

# Using spdlog for logging
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

target_link_libraries(${PROJECT_NAME} INTERFACE OpenSSL::SSL spdlog::spdlog)

#[[
------------------
Tests and Examples
------------------
]]

# Check if Hamarr is being used as the main project vs. via sub_directory() or
# similar. We'll use this to enable or disable tests, installation, etc.
if(NOT DEFINED HAMARR_MAIN_PROJECT)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(HAMARR_MAIN_PROJECT ON)
  else()
    set(HAMARR_MAIN_PROJECT OFF)
  endif()
endif()

option(BUILD_HAMARR_TESTS "Build Hamarr tests" ${HAMARR_MAIN_PROJECT})

include(CTest) # Provides the BUILD_TESTING option (ON by default)
if(((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) OR BUILD_HAMARR_TESTS)
   AND BUILD_TESTING)
  message("Hamarr tests will be built...")
  enable_testing()
  add_subdirectory(test)
endif()

option(BUILD_HAMARR_EXAMPLES "Build Hamarr examples" ${HAMARR_MAIN_PROJECT})

if(BUILD_HAMARR_EXAMPLES)
  message("Hamarr examples will be built...")
  add_subdirectory(examples)
endif()

#[[
--------------------
Library Installation
--------------------
]]

option(INSTALL_HAMARR "Enable installation of Hamarr library"
       ${HAMARR_MAIN_PROJECT})

if(INSTALL_HAMARR)
  message("Configuring library installation...")

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

  # TODO: Properly implement CPack stuff for package creation
  # set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
  # include(CPack)

endif()


message("CMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}")