cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(SistemaIoT VERSION 1.0 LANGUAGES CXX)

# Estandar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir directorios de cabeceras
include_directories(${CMAKE_SOURCE_DIR}/include)

# Ejecutable principal
add_executable(SistemaIoT main.cpp)

# Mensaje de confirmacion
message(STATUS "Proyecto SistemaIoT configurado correctamente")
