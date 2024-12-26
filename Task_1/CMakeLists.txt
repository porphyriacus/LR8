cmake_minimum_required(VERSION 3.5.0)
project(task1 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_executable(task1 customer.c main.c )


# Подключение библиотеки libuuid и libcurl
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)
pkg_check_modules(CURL REQUIRED libcurl)

target_include_directories(task1 PRIVATE ${UUID_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
target_link_libraries(task1 ${UUID_LIBRARIES} ${CURL_LIBRARIES})
# Добавьте библиотеку json-c
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONC REQUIRED json-c)
include_directories(${JSONC_INCLUDE_DIRS})
target_link_libraries(task1 ${JSONC_LIBRARIES})



