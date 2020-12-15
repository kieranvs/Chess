cmake_minimum_required(VERSION 3.12)
project(Chess VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======== Googletest ========
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(dependencies/googletest)

# ======== Chess ========
add_library(
	Chess
	Chess/Placeholder.cpp
)
target_include_directories(Chess PRIVATE . dependencies)

# ======== UnitTest ========
add_executable(
	UnitTest
	Tests/TestPlaceholder.cpp
	Tests/UnitTestMain.cpp
)
target_include_directories(UnitTest PRIVATE . dependencies)
target_link_libraries(UnitTest PRIVATE gtest Chess)