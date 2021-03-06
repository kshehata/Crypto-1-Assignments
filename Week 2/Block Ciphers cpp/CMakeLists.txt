cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("Block Ciphers")

enable_testing()
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(utils utils.cc)
add_executable(UtilsTest utils_test.cc)
add_test(UtilsTest bin/UtilsTest)
target_link_libraries(UtilsTest utils ${CONAN_LIBS})

add_library(block_cipher_lib block_cipher.cc)
add_executable(BlockCipherTest block_cipher_test.cc)
add_test(BlockCipherTest bin/BlockCipherTest)
target_link_libraries(BlockCipherTest block_cipher_lib utils ${CONAN_LIBS})

add_executable(blocks main.cc)
target_link_libraries(blocks utils block_cipher_lib ${CONAN_LIBS})
