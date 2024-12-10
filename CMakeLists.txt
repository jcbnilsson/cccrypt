cmake_minimum_required(VERSION 3.31)

project(cccrypt VERSION 0.1.0
        DESCRIPTION "Simple program encoding and decoding Geometry Dash CCGameManager.dat files"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ZLIB REQUIRED)

add_compile_definitions(CCCRYPT_VERSION="${PROJECT_VERSION}")

add_executable(cccrypt
        src/cccrypt.cpp
        src/CryptManager.cpp
)

target_link_libraries(
        cccrypt
        PRIVATE
        ZLIB::ZLIB
)

install(TARGETS cccrypt
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)