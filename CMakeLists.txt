cmake_minimum_required(VERSION 3.13)

project(anothergbemulator)

add_library(anothergbemulator STATIC 
    src/cpu/instruction.cpp
)

target_include_directories(anothergbemulator
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
	PRIVATE
		${PROJECT_SOURCE_DIR}/include/cpu
		${PROJECT_SOURCE_DIR}/include/memory
		${PROJECT_SOURCE_DIR}/include/utils
)

set(CMAKE_CXX_STANDARD 17)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE anothergbemulator)