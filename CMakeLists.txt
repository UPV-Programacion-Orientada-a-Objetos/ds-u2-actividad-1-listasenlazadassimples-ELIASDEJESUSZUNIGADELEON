cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(SistemaIoT VERSION 1.0 LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ejecutable principal
add_executable(SistemaIoT main.cpp)

# Mensaje de confirmación
message(STATUS "Proyecto SistemaIoT configurado correctamente")
