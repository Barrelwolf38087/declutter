cmake_minimum_required(VERSION 3.17)
project(declutter)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB taglib)

add_executable(declutter main.cpp)

target_include_directories(declutter PRIVATE ${TAGLIB_INCLUDE_DIRS})
target_link_libraries(declutter PRIVATE ${TAGLIB_LIBRARIES})

target_include_directories(declutter PRIVATE "${CMAKE_SOURCE_DIR}/include/")

install(TARGETS declutter RUNTIME DESTINATION bin)
