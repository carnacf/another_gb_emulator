cmake_minimum_required(VERSION 3.21)

project(anothergbemulator)

set(CMAKE_CXX_STANDARD 20)

add_library(anothergbemulator STATIC 
 "src/cpu/executor.cpp"
 "src/cpu/tracer.cpp"
 "include/cpu/executor.h"
 "include/cpu/executor-impl.hpp"
 "include/cpu/registery.h"
 "include/cpu/tracer.h"
 "include/cpu/tracer-impl.hpp"
 "include/memory/memory.h"
 "include/utils/global.h"
 "include/utils/utils.h"
 "include/cpu/instruction_utils.h")

target_include_directories(anothergbemulator
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
	PRIVATE
		${PROJECT_SOURCE_DIR}/include/cpu
		${PROJECT_SOURCE_DIR}/include/memory
		${PROJECT_SOURCE_DIR}/include/utils
)

add_subdirectory(tests)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE anothergbemulator)


