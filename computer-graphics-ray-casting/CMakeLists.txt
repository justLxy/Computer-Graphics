cmake_minimum_required(VERSION 3.20)
project(raycasting)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Toggle solution vs. src
option(USE_SOLUTION "Build the reference solution" OFF)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
if (USE_SOLUTION)
  set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/solution")
endif()

# Sources (use CONFIGURE_DEPENDS so CMake reconfigures when files change)
file(GLOB SRCFILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

# Create target executable
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" ${SRCFILES})

# Include paths
target_include_directories(${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Eigen under ./eigen/
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/eigen")
  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/eigen")
else()
  message(WARNING "Eigen not found. Install Eigen3 or provide a ./eigen directory.  You may need to call git submodule update --init --recursive.")
endif()

# Warnings
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
