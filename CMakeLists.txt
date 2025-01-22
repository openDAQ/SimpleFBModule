cmake_minimum_required(VERSION 3.25)
set(REPO_NAME example_fb_module)
set(REPO_OPTION_PREFIX EXAMPLE_MODULE)

project(${REPO_NAME} VERSION 1.0.0)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MESSAGE_CONTEXT ${REPO_NAME})
set(CMAKE_MESSAGE_CONTEXT_SHOW ON CACHE BOOL "Show CMake message context")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_definitions(-DFMT_HEADER_ONLY)

option(OPENDAQ_FB_EXAMPLE_ENABLE_APP "Enable building example function block application" OFF)
option(EXAMPLE_MODULE_ENABLE_TESTS "Enable building of test suite for the example function block module" OFF)

include(CommonUtils)
setup_repo(${REPO_OPTION_PREFIX})

if(OPENDAQ_FB_EXAMPLE_ENABLE_APP)
	set(DAQMODULES_REF_FB_MODULE ON CACHE BOOL "" FORCE)
	set(DAQMODULES_REF_DEVICE_MODULE ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(external)
add_subdirectory(example_module)

if(OPENDAQ_FB_EXAMPLE_ENABLE_APP)
	message(STATUS "Enabled example function block application")
    add_subdirectory(example_application)
endif()

# Set CPack variables
set(CPACK_COMPONENTS_ALL RUNTIME)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/package")

# Set the CPack generator based on the platform
if (WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif (UNIX AND NOT APPLE)
    cmake_host_system_information(RESULT DISTRO_ID QUERY DISTRIB_ID)
    cmake_host_system_information(RESULT DISTRO_VERSION_ID QUERY DISTRIB_VERSION_ID)
    set(CPACK_SYSTEM_NAME "${DISTRO_ID}${DISTRO_VERSION_ID}")
    set(CPACK_GENERATOR "TGZ")
endif()

# Include CPack for packaging
include(CPack)