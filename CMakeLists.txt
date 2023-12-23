cmake_minimum_required(VERSION 3.26)
project(so2_hw_2024 C)

set(CMAKE_C_STANDARD 11)

add_executable(so2_hw_2024
#        single-process-version/hw2024sp.c
#        single-process-version/read_text_file.c
#        single-process-version/to_lower.c
#        single-process-version/list_operations.c
#        single-process-version/write_output.c
#        single-process-version/common.h
#        single-process-version/round_down_to_decimal.c
#        single-process-version/write_text_file.c
#        single-process-version/read_csv_and_build_list.c
        multi-process-version/hw2024mp.c
        common/common.h
)
find_package(GSL REQUIRED)
target_include_directories(so2_hw_2024 PRIVATE ${GSL_INCLUDE_DIRS})
target_link_libraries(so2_hw_2024 PRIVATE ${GSL_LIBRARIES})