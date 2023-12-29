cmake_minimum_required(VERSION 3.26)
project(so2_hw_2024 C)

set(CMAKE_C_STANDARD 11)

add_executable(so2_hw_2024
        common/write_text_file.c
        common/read_csv_and_build_list.c
        common/common.h
        common/to_lower.c
        common/write_output.c
        common/list_operations.c
        common/read_text_file.c
        common/round_down_to_decimal.c
        multi-process-version/hw2024mp.c
        multi-process-version/serialize_list.c
        multi-process-version/deserialize_list.c
        multi-process-version/free_list.c
        multi-process-version/reverse_list.c
        #        single-process-version/hw2024sp.c
)
find_package(GSL REQUIRED)
target_include_directories(so2_hw_2024 PRIVATE ${GSL_INCLUDE_DIRS})
target_link_libraries(so2_hw_2024 PRIVATE ${GSL_LIBRARIES})