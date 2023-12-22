cmake_minimum_required(VERSION 3.26)
project(so2_hw_2024 C)

set(CMAKE_C_STANDARD 11)

add_executable(so2_hw_2024
        hw2004st.c)

include_directories(C:/dev/gnuwin32/include)
link_directories(C:/dev/gnuwin32/lib)

# Nota: Rimuovi il prefisso "lib" nei nomi delle librerie
target_link_libraries(so2_hw_2024 gsl gslcblas m)
