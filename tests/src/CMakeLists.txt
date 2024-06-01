cmake_minimum_required(VERSION 3.20)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/tests/include/*.hpp")
add_library(tests random.cpp additions.cpp decode.cpp errors.cpp logger.cpp utils.cpp ${HEADER_LIST})
target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/tests/include)
target_link_libraries(tests tfhe)

add_executable(test main.cpp)
target_link_libraries(test tfhe)
target_link_libraries(test tests)
