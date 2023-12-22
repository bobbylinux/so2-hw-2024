cmake_minimum_required(VERSION 3.26)
project(so2_hw_2024 C)

set(CMAKE_C_STANDARD 11)

add_executable(so2_hw_2024
        hw2004st.c
        )

find_package(GSL REQUIRED)
target_include_directories(so2_hw_2024 PRIVATE ${GSL_INCLUDE_DIRS})
target_link_libraries(so2_hw_2024 PRIVATE ${GSL_LIBRARIES})