cmake_minimum_required(VERSION 3.20)
project(yuki.utils LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    message("No build type specified. Defaulted to 'Release'.")
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(YUKI_UTILS_DBG TRUE)
    add_compile_options(-Wextra -O0)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wno-missing-field-initializers)
    elseif(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        message(FATAL_ERROR "Unsupported compiler! Please use GCC or Clang!")
    endif()
    message("Build type: Debug")
else()
    set(YUKI_UTILS_DBG FALSE)
    add_compile_options(-O2)
    message("Build type: Release")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include /usr/local/include ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
link_directories(/usr/local/lib)

add_executable(ugen ugen/ugen.cpp)
if(YUKI_UTILS_DBG)
    target_compile_options(ugen PUBLIC -DYUKI_UGEN_DBG)
endif()

add_custom_target(build-time-make-directory
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/lib/unicode)

add_custom_command(
    OUTPUT
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/general_category.h
        ${PROJECT_SOURCE_DIR}/lib/unicode/general_category.cpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/scripts.h
        ${PROJECT_SOURCE_DIR}/lib/unicode/scripts.cpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/blocks.hpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/binary_properties.h
        ${PROJECT_SOURCE_DIR}/lib/unicode/binary_properties.cpp
        ${PROJECT_SOURCE_DIR}/lib/unicode/case.cpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/mirror.hpp
    COMMAND ugen ugen include/yuki/unicode lib/unicode
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS ugen build-time-make-directory
    COMMENT "ugen-ing")

add_custom_target(ugen_output
    DEPENDS
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/general_category.h
        ${PROJECT_SOURCE_DIR}/lib/unicode/general_category.cpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/scripts.h
        ${PROJECT_SOURCE_DIR}/lib/unicode/scripts.cpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/blocks.hpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/binary_properties.h
        ${PROJECT_SOURCE_DIR}/lib/unicode/binary_properties.cpp
        ${PROJECT_SOURCE_DIR}/lib/unicode/case.cpp
        ${PROJECT_SOURCE_DIR}/include/yuki/unicode/mirror.hpp)

add_library(yuki_unicode
    lib/unicode/general_category.cpp
    lib/unicode/scripts.cpp
    lib/unicode/binary_properties.cpp
    lib/unicode/case.cpp)

add_library(yuki_uchar_rare_encodings lib/uchar_rare_encodings.cpp)

install(DIRECTORY include/yuki TYPE INCLUDE)
install(TARGETS yuki_unicode yuki_uchar_rare_encodings)
