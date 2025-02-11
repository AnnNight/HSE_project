cmake_minimum_required(VERSION 3.21)
project(game)

set(CMAKE_CXX_STANDARD 14)

include_directories(SFML/include)

add_executable(game main.cpp Player.cpp Player.h Game.cpp Game.h Map.cpp Map.h Bullet.cpp Bullet.h Audio.cpp Audio.h Game_time.cpp Game_time.h Enemy_tanks.cpp Enemy_tanks.h Entity.h Entity.cpp Menu.h Types.h Menu.cpp)

set(SFML_ROOT SFML)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(game ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()