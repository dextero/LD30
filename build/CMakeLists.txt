cmake_minimum_required(VERSION 2.8.5)

project(LD30)

include(${CMAKE_SOURCE_DIR}/utils.cmake)

# project properties
set(ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)

if(NOT DEFINED ENV{SDK_DIR})
    set(ENV{SDK_DIR} "${ROOT_DIR}/extlibs")
endif()

set(SFML_ROOT "$ENV{SDK_DIR}/SFML")

# setup library directories
if(DEFINED ENV{SDK_DIR})
    file(TO_CMAKE_PATH $ENV{SDK_DIR} SDK_DIR)
    find_library_dir(${SDK_DIR} "SFML")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

# external libraries
set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS system window graphics audio)

include_directories(${SFML_INCLUDE_DIR})
set(LIBS ${LIBS} ${SFML_LIBRARIES})

# project sources
include_directories(${ROOT_DIR}/src)
find_sources(GAME_HEADERS ${ROOT_DIR}/src ".h" "include")
find_sources(GAME_CPP_SOURCES ${ROOT_DIR}/src ".cpp" "src")
set(GAME_SOURCES ${GAME_HEADERS}
                    ${GAME_CPP_SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -Wno-variadic-macros -Wno-pragmas -Wno-unknown-pragmas -DGLM_FORCE_RADIANS -DGLM_SWIZZLE")

add_executable(game ${GAME_SOURCES})
target_link_libraries(game ${LIBS})

