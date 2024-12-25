cmake_minimum_required(VERSION 3.5.0)
project(task1 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_executable(task1 customer.c file.c input.c main.c menu.c print.c elastic.c)

# Добавьте библиотеку libcurl
find_package(CURL REQUIRED)
target_link_libraries(task1 CURL::libcurl)
# Добавьте библиотеку json-c
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONC REQUIRED json-c)
include_directories(${JSONC_INCLUDE_DIRS})
target_link_libraries(task1 ${JSONC_LIBRARIES})
