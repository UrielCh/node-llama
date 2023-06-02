cmake_minimum_required(VERSION 3.10)
project(node-llama)

set(CMAKE_CXX_STANDARD 14)

add_library(node-llama SHARED
    binding.cpp)

set_target_properties(node-llama PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(node-llama PRIVATE
    ${CMAKE_JS_INC}
    ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api)

target_link_libraries(node-llama PRIVATE
    ${CMAKE_JS_LIB}
    llama)

add_definitions(-DNAPI_VERSION=8)

# Add this line to include the llama subdirectory
add_subdirectory(llama)
