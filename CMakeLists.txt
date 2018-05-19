# para usarlo:
# > mkdir build 
# > cd build
# > cmake -G "Unix Makefiles"
# > make


# minima version de CMake
cmake_minimum_required(VERSION 3.7)

# nombre del proyecto
project(tp)

# SDL
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# crea ejecutable(nombre main.cpp dependencia.cpp dependencia (sin.h)...)
# add_executable(tp prueba.cpp Imagen.cpp Imagen Control_Animacion.cpp Control_Animacion)

#toma todos los ejecutables
file(GLOB tp_SRC
    "*.h"
    "*.cpp"
)
add_executable(tp ${tp_SRC})



# likea con SDL
target_link_libraries(tp ${SDL2_LIBRARIES})

