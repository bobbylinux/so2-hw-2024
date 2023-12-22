cmake_minimum_required(VERSION 3.26)
project(so2_hw_2024 C)

set(CMAKE_C_STANDARD 11)

add_executable(so2_hw_2024
        hw2024sp.c
        read_text_file.c
        to_lower.c
        list_operations.c
        write_output.c
        common.h
        round_down_to_decimal.c
        write_text_file.c
        read_csv_and_build_list.c
        hw2024mp.c
)

find_package(GSL REQUIRED)
target_include_directories(so2_hw_2024 PRIVATE ${GSL_INCLUDE_DIRS})
target_link_libraries(so2_hw_2024 PRIVATE ${GSL_LIBRARIES})