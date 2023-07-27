cmake_minimum_required(VERSION 3.21)

project(anothergbemulator)

set(CMAKE_CXX_STANDARD 20)

add_library(anothergbemulator STATIC 
 "src/cpu/processor.cpp"
 "src/cpu/logger.cpp"
 "src/memory/cartridge.cpp" 
 "src/memory/memory.cpp" 
 "src/memory/mmio.cpp"
 "include/cpu/processor.h"
 "include/cpu/processor-impl.hpp"
 "include/cpu/registery.h"
 "include/cpu/logger.h"
 "include/cpu/logger-impl.hpp"
 "include/memory/memory.h"
 "include/utils/global.h"
 "include/utils/utils.h"
 "include/cpu/instruction_utils.h" 
 "include/memory/rom.h" 
 "include/video/screen.h" 
 "include/memory/mmio.h" 
 "src/video/screen.cpp")

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

add_subdirectory(3rd_parties)

target_link_libraries(app 
    PRIVATE 
        anothergbemulator
        sfml-audio
        sfml-graphics
        sfml-system
        sfml-window
)
