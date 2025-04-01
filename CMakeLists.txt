cmake_minimum_required(VERSION 3.30)
project(InclinedPlaneSimulation C)

set(CMAKE_C_STANDARD 23)

add_executable(simulate
        src/main.c src/main.h
        src/simulate.c src/simulate.h
        src/csv_output.c src/csv_output.h
        src/gl_trail.c src/gl_trail.h
)

target_link_libraries(simulate
        GL glut glfw
        m
)
