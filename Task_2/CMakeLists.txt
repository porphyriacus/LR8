cmake_minimum_required(VERSION 3.5.0)
project(task2 VERSION 0.1.0 LANGUAGES C)

add_executable(task2 call.c main.c)

# Добавьте библиотеку libcurl
find_package(CURL REQUIRED)
target_link_libraries(task2 CURL::libcurl)
# Добавьте библиотеку json-c
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONC REQUIRED json-c)
include_directories(${JSONC_INCLUDE_DIRS})
target_link_libraries(task2 ${JSONC_LIBRARIES})
