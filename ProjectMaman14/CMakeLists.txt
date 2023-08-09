cmake_minimum_required(VERSION 3.26)
project(ProjectMaman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(ProjectMaman14
        program.h
        program.c
        macro.c
        macro.h
        firstPass.c
        firstPass.h
        secondPass.c
        secondPass.h
        labels.h

)
