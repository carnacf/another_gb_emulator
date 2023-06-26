include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests 
	main_tests.cpp
	utils_tests.cpp
	registers_tests.cpp)

target_link_libraries(tests anothergbemulator gtest)
include(GoogleTest)

gtest_discover_tests(tests)