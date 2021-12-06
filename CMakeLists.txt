cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
project("downloader" LANGUAGES CXX)

find_package(PkgConfig)
pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)

set(BINARY_NAME downloader)
add_executable(${BINARY_NAME}
  "src/main.cc"
  "src/downloader.cc"
  "src/cache.cc"
)

target_include_directories(${BINARY_NAME} PRIVATE "src")
target_include_directories(${BINARY_NAME} PRIVATE PkgConfig::CURL)
target_link_libraries(${BINARY_NAME} PRIVATE PkgConfig::CURL)
