cmake_minimum_required(VERSION 3.9)
project(Algo-3-TP3)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O3 -Wextra -Wall")
#set(CMAKE_CXX_FLAGS "-Wextra -Wall")


add_executable(TP3_Jugador_Goloso mainJugadorGoloso.cpp JugadorGoloso.cpp)

add_executable(TP3_Jugador_Random_Profes "./ZIP Profes/TP3/src/random_player.cpp")

add_executable(TP3_Genetico TorneosGeneticos.cpp JugadorGoloso.cpp)

#add_executable(TP3_Optimizacion_Otro)

#add_executable(TP3_experimentacion)