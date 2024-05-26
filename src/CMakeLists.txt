cmake_minimum_required(VERSION 3.20)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.hpp")
add_library(tfhe generator.cpp common.cpp domain_expansion.cpp tgsw.cpp tlwe.cpp ${HEADER_LIST})
target_include_directories(tfhe PUBLIC ${PROJECT_SOURCE_DIR}/include)
