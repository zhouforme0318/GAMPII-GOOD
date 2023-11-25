cmake_minimum_required(VERSION 3.10)
project(run_GOOD)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions( -D_USE_MATH_DEFINES=True)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(WINDOWS TRUE)
else()
    set(CMAKE_CXX_FLAGS "-Wno-unused-result")
endif()

file(GLOB_RECURSE SOURCE
        src/core/config.cpp
        src/core/ftps.cpp
        src/run_GOOD.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} ${SOURCE})

if (NOT WINDOWS)
    find_package(PkgConfig QUIET)
    pkg_check_modules(YAML_CPP yaml-cpp QUIET)

    if (YAML_CPP_FOUND)
        target_link_libraries(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${YAML_CPP_INCLUDE_DIRS})
    else()
        message(WARNING "yaml-cpp not found. Building from local source...")
        add_subdirectory(src/thirdparty/yaml-cpp-0.7.0)
        target_link_libraries(${PROJECT_NAME} yaml-cpp)
    endif()
endif()
