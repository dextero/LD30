cmake_minimum_required(VERSION 3.1)
project(LD30 LANGUAGES CXX)

# project properties
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib" CACHE PATH "")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin" CACHE PATH "")

string(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-variadic-macros -Wno-pragmas -Wno-unknown-pragmas" CACHE STRING "")

# external libraries
find_package(SFML 2 REQUIRED COMPONENTS system window graphics audio)

add_executable(game
               src/asteroid.cpp
               src/asteroid.h
               src/audio.cpp
               src/audio.h
               src/config.h
               src/explosion.cpp
               src/explosion.h
               src/game.cpp
               src/game.h
               src/game_over_screen.cpp
               src/game_over_screen.h
               src/game_screen.cpp
               src/game_screen.h
               src/main.cpp
               src/menu_screen.cpp
               src/menu_screen.h
               src/planet.cpp
               src/planet.h
               src/popup_messages.cpp
               src/popup_messages.h
               src/powerup.h
               src/screen.cpp
               src/screen.h
               src/sun.cpp
               src/sun.h
               src/utils.h)
target_compile_definitions(game PRIVATE
                           $<$<CONFIG:Debug>:-D_DEBUG>
                           $<$<CONFIG:Release>:-DNDEBUG>
                           -DGLM_FORCE_RADIANS -DGLM_SWIZZLE)
target_include_directories(game PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}/src"
                           "${SFML_INCLUDE_DIR}")
target_link_libraries(game PRIVATE ${SFML_LIBRARIES})

