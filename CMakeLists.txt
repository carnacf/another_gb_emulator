cmake_minimum_required(VERSION 3.21)

project(anothergbemulator)

set(CMAKE_CXX_STANDARD 20)

add_library(anothergbemulator STATIC 
    "src/cpu/executor.cpp"
)

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


